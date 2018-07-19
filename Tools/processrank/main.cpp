#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <time.h>
using namespace std;

struct SCtrInfo
{
    SCtrInfo() 
    {
        pv = 0;
        click = 0;
    }
    uint32_t pv;
    uint32_t click; 
    float ctr;
};

struct SRankOutput
{
    uint32_t id;
    string name;
	size_t range; // rank value range
};

SCtrInfo  allctr[21][1002];

const uint32_t splitcount = 20;
uint32_t allpv;

SRankOutput ad_outputs[] =
{
    {1, "relevance", 100},
    {3, "content", 100},
    {5, "absolute", 100},
    {6, "match_feature", 100},
    {7, "match_cover", 100},
    {8, "ad_ext", 100},
    {9, "url_ext", 100},
    {10, "concur", 100},
    {13, "keyword", 100},
    {14, "title", 100},
    {15, "desc", 100},
    {16, "title_highlight", 100},
    {17, "desc_highlight", 100},
    {18, "only_match", 100},
    {19, "only_nonmatch", 100},
    {20, "match_nonmatch", 100},

};
size_t ad_rank_num = sizeof(ad_outputs)/sizeof(SRankOutput);

SRankOutput web_outputs[] =
{
	{1, "fixed_rank", 1},
	{2, "content_rank", 100},
	{3, "key_bm25_rank", 10},
	{4, "bm25_rank", 100},
	{5, "match_rank", 100},
	{6, "page_rank", 256},
	{7, "user_rank", 128},
	{8, "time_rank", 10},
	{9, "rank", 2000},
	{10, "final_rank", 10}
};
size_t web_rank_num = sizeof(web_outputs)/sizeof(SRankOutput);

const SRankOutput * outputs = web_outputs;
const size_t rank_num = web_rank_num;
const int rank_merge_num = 5; // 有两个rank合在一个域内，需要分开，若为-1，表示没有rank合在一起

struct SLogInfo
{
    string      suid;
    string      yyid;
    uint32_t    suidcount;
    uint32_t    yyidcount;
    uint32_t    isClick;
};

string GetFullPath(const char* dir, const char* filename)
{
    ostringstream oss;
    oss << dir;
    if (dir[strlen(dir)-1] != '/')
    {
        oss << '/';
    }
    oss << filename;
    return oss.str();
}

void SplitString(const string &src, char delimitor, vector<string> &dest, bool ignoreEmpty)
{
    dest.clear();
    string::size_type currentPos = 0;
    string::size_type findPos = src.find(delimitor);

    while(findPos != string::npos)
    {        
        if (findPos != currentPos)
        {
            dest.push_back(src.substr(currentPos, findPos - currentPos));
        }
        else if (!ignoreEmpty)
        {
            dest.push_back("");
        }
        currentPos = findPos + 1;
        findPos = src.find(delimitor, currentPos);
    }
    
    if (currentPos != src.size())
    {
        dest.push_back(src.substr(currentPos));
    }
    else if (!ignoreEmpty)
    {
        dest.push_back("");
    }
}

uint32_t ParseIp(const string &ipstring)
{
    vector<string> addr;
    SplitString(ipstring, '.', addr, false);
    if (addr.size() != 4 )
    {
        return 0;
    }
    return (atoi(addr[0].c_str()) << 24) + (atoi(addr[1].c_str()) << 16) + (atoi(addr[2].c_str()) << 8) + atoi(addr[3].c_str());
    
}

void OutputCtrInfo(uint32_t id, const string &outputdir, const string &filename)
{
    string outputPath = GetFullPath(outputdir.c_str(), filename.c_str());
    cout << outputPath << endl;
    ofstream ofs(outputPath.c_str());
    if (ofs == NULL)
    {
        return;
    }
    
    uint32_t orgpv = allpv;

    if (allctr[id][1001].pv != 0)
    {
        ofs << "-1,\t" << allctr[id][1001].pv << "\t" << allctr[id][1001].click << "\t" << float(allctr[id][1001].click) / allctr[id][1001].pv << endl;
        orgpv = orgpv - allctr[id][1001].pv;
    }        

    bool isOK = false;
    
    while (!isOK && orgpv * 5 > allpv)
    {
        isOK = true;        
        uint32_t currentPv = orgpv; 
        for (uint32_t i = 0; i <= 1000; i++)
        {
            if (allctr[id][i].pv * splitcount * 2 >= currentPv && allctr[id][i].pv > 0)
            {
                ofs << float(i) / 10 << ",\t" << allctr[id][i].pv << "\t" << allctr[id][i].click << "\t" << float(allctr[id][i].click) / allctr[id][i].pv << endl;
                orgpv = orgpv - allctr[id][i].pv;
                allctr[id][i].pv = 0;
                allctr[id][i].click = 0;
                isOK=false;
            }
        }
    }
    uint32_t currentpv = 0;
    uint32_t areapv = 0;
    uint32_t areaclick = 0;
    uint32_t startscore = 0;
    uint32_t currentpos = 1;
    
    while (allctr[id][startscore].pv == 0)
    {
        startscore++;
    }
    for (uint32_t i = 0; i <= 1000; i++)
    {
        areapv += allctr[id][i].pv;
        areaclick += allctr[id][i].click;

        currentpv += allctr[id][i].pv;
        if (currentpv >= orgpv * currentpos / splitcount)
        {
            if (areapv != 0)
            {
                ofs << float(startscore) / 10 << "," << float(i) / 10 << "\t" << areapv << "\t" << areaclick << "\t" << float(areaclick) / areapv << endl;
            }
            currentpos++;
            startscore = i;
            areapv = 0;
            areaclick = 0;
        }
    }
    ofs.close();
}


uint32_t GetBucket(const string &str, size_t range)
{
    float score = atof(str.c_str());
	score *= (100.0f/range); // normalized to 0~100
    if (score < 0)
    {
        return 1001;
    } 
    uint32_t iscore = uint32_t(score * 10);
    if (iscore > 1000)
    {
        iscore = 1000;
    }
    return iscore;
}

uint32_t GetBucket(const uint32_t &rank, size_t range)
{
    float score = (float)rank;
	score *= (100.0f/range); // normalized to 0~100
    if (score < 0)
    {
        return 1001;
    } 
    uint32_t iscore = uint32_t(score * 10);
    if (iscore > 1000)
    {
        iscore = 1000;
    }
    return iscore;
}

void ParseGeneralInformation(const string &inputFileName, uint32_t type)
{
    ifstream ifs(inputFileName.c_str());
	if (!ifs)
	{
		cerr << "open " << inputFileName << " failed\n";
		exit(-1);
	}

    string line;
    uint32_t count = 0;
    while (getline(ifs, line))
    {
        count++;
        if (count % 100000 == 0)
        {
            cerr << count <<endl;
        }

        vector<string> elements;
        SplitString(line, '', elements, false);

		size_t field_num = rank_merge_num < 0 ? 11+rank_num : 10+rank_num;
        if (elements.size() < field_num)
        {
            continue;
        }

        uint32_t isClick     = atoi(elements[0].c_str());
        uint32_t adPos       = atoi(elements[7].c_str());

        //uint32_t relevance   = atoi(elements[18].c_str());
        if (adPos != type)
        {
            continue;
        }

        allpv++;
		int index = 0;
        for (int i = 0; i < rank_num;)
        {
			uint32_t bucket = 0;
			if (i == rank_merge_num)
			{
				uint32_t rank = atoi(elements[10 + index].c_str());
				rank /= 256;
				bucket = GetBucket(rank, outputs[i].range);
			}
			else if (i== rank_merge_num+1)
			{
				uint32_t rank = atoi(elements[10 + index].c_str());
				rank = rank%128;
				bucket = GetBucket(rank, outputs[i].range);
				index++;
			}
			else
			{
				bucket = GetBucket(elements[10 + index], outputs[i].range);
				index++;
			}
            allctr[outputs[i].id][bucket].pv ++;
            if (isClick == 1)
            {
                allctr[outputs[i].id][bucket].click ++;
            }
			i++;
        }
        
    }
    ifs.close();        
}

int main(int argc,char** argv)
{
    if (argc != 4)
    {
		cerr << "Usage: " << argv[0] << " sample_file_list out_dir pos\n";
        return 0;
    }
    
    string logFileNameList = argv[1];
    string outpath = argv[2];
    uint32_t type = atoi(argv[3]);

    allpv = 0; 

    ifstream ifs(logFileNameList.c_str());
    string line;
    while (getline(ifs,line))
    {
        cout << line << endl;
        ParseGeneralInformation(line, type);
    }

    for (uint32_t i = 0; i < rank_num; i++)
    {
        OutputCtrInfo(outputs[i].id, outpath, outputs[i].name);
    }

    return 0;
}
