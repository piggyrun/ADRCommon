#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

#include "RegDataCenter.h"
#include "WordSegmentor3/Segmentor/DicTree.h"
#include "WordSegmentor3/CommonSegmentor/WordSegmentor.h"

const char* CATE_INDEX_FILE = "category_index";
const char* CATE_HIGH_INDEX_FILE = "category_high_Index";
const uint32_t DEFAULT_CATE_INDEX = 0;


CRegDataCenter* CRegDataCenter::_instance = 0;


CRegDataCenter* CRegDataCenter::Instance()
{
    if (_instance == 0)
    {
        _instance = new CRegDataCenter();
    }
    return _instance;
}

CRegDataCenter::CRegDataCenter()
    :_logFunc(NULL),
     _errorFunc(NULL),
     _cate2index(300),
     _cate2indexHigh(50),
     _cateCount(0),
     _cateCountHigh(0),
     _avgCtr(DEFAULT_AVG_CTR)

{
}

int CRegDataCenter::Initialize(const string &modelPath, EAppScenario scenario)
{
	// 加载分词用的dictree文件
	string dic_filename = GetFullPath(modelPath.c_str(), "dicmap.bin");
	if (analysis::DicTree::instance()->loadDic(dic_filename.c_str()) == false)
	{    
		fprintf(stderr, "[Load dictree file %s failed]\n", dic_filename.c_str());
		throw;
	}    
	// 加载encoding表
	string encoding_path = GetFullPath(modelPath.c_str(), "encoding");
	if (EncodingConvertor::getInstance()->initializeInstance(encoding_path.c_str()) < 0) 
	{    
		fprintf(stderr, "[Initialize EncodingConverter in path %s failed]\n", encoding_path.c_str());
		throw;
	}
	if (SCENARIO_AD == scenario)
	{
		if (LoadCate2Id(modelPath, CATE_INDEX_FILE, _cate2index, _cateCount) != 0)
		{
			throw;
		}
		if (LoadCate2Id(modelPath, CATE_HIGH_INDEX_FILE, _cate2indexHigh, _cateCountHigh) != 0)
		{
			throw;
		}
	}
    return 0;
}

void CRegDataCenter::LoggingNormal(const char *content, ...) const
{
    if (_logFunc != NULL)
    {
        va_list args;
        va_start(args, content);    
        char buffer[256];
        vsnprintf(buffer, 256, content, args);
        va_end(args);         
        
        _logFunc(buffer);
        
    }
}

void CRegDataCenter::LoggingError(const char *content, ...) const
{
    if (_errorFunc != NULL)
    {
        va_list args;
        va_start(args, content);    
        char buffer[256];
        vsnprintf(buffer, 256, content, args);        
        va_end(args);    

        _errorFunc(buffer); 
    }
}


void CRegDataCenter::SetLogFunc(FuncPtr funcPtr)
{
    _logFunc = funcPtr;
}

void CRegDataCenter::SetErrorFunc(FuncPtr funcPtr)
{
    _errorFunc = funcPtr;
}


int CRegDataCenter::LoadCate2Id(const string &modelPath, const char *fileName, Int2IntHash &indexMap, uint32_t &count)
{
    ifstream ifs(GetFullPath(modelPath.c_str(), fileName).c_str());

    if (ifs == NULL)
    {        
        return -1;
    }

    count = 1;
    indexMap.unbind_all();

    string line;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() != 2)
        {
            continue;
        }
        //uint32_t index = atoi(elements[0].c_str());
        uint32_t category = atoi(elements[1].c_str());
        indexMap.bind(category, count);
        count++;
    }
    return 0;
}

uint32_t CRegDataCenter::GetCateIndex(uint32_t cate) const
{
    return GetIndex(cate, _cate2index);
}

uint32_t CRegDataCenter::GetCateIndexHigh(uint32_t cate) const
{
    return GetIndex(cate/10000, _cate2indexHigh);
}

uint32_t CRegDataCenter::GetIndex(uint32_t cate, const Int2IntHash &indexMap) const
{
    uint32_t value;
    if (indexMap.find(cate, value) == -1)
    {
        return DEFAULT_CATE_INDEX;
    }
    return value;
}

int CRegDataCenter::do_segment(const char * query, vector<t_segment_res> &seg_res) const
{
	char buf[65536];
	analysis::TTermInfo term_list[100000];

	// inital segmentor
	analysis::WordSegmentor *word_segmentor = new analysis::WordSegmentor;///分词器
	word_segmentor->open(analysis::DicTree::instance());

	seg_res.clear();

	memset(buf, 0, sizeof(buf));
	EncodingConvertor * ec = EncodingConvertor::getInstance();
	int len = ec->t2sgbk(query, strlen(query), buf, sizeof(buf));
	if (len >= 65536)
		return -1;
	buf[len] = '\0';

	int slen = strlen(buf)/2;
	int term_num = word_segmentor->simpleSegment((gchar_t*)buf, slen, term_list, 100000);
	if (term_num < 0)
	{
		//fprintf(stderr, "[WordSegmentation] term_num illegal:%d\n", term_num);
		return -1;
	}
	// 处理分词结果
	for (int i = 0; i < term_num; i++)
	{
		t_segment_res res;
		/*
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, &buf[0], term_list[i].pos * 2);
		EncodingConvertor::getInstance()->sbc2dbc(tmp, tmp, sizeof(tmp));
		res.pos = strlen(tmp);
		
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, &buf[term_list[i].pos * 2], term_list[i].len * 2);
		EncodingConvertor::getInstance()->sbc2dbc(tmp, tmp, sizeof(tmp));
		res.len = strlen(tmp);
		*/
		
		res.id = term_list[i].id;

		/*// get text
		char txt[64];
		strncpy(tmp, buf+term_list[i].pos*2, term_list[i].len*2); 
		EncodingConvertor::getInstance()->sbc2dbc(tmp, txt, 64, true);
		termid_txt_map[res.id] = txt;
		//*/

		seg_res.push_back(res);
	}
	delete word_segmentor;

	return 0;
}
