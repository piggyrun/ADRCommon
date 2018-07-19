#include <fstream>
#include "CrossFeature.h"
#include "RegressionLib/RegDataCenter.h"

namespace
{
    const char * MODULE_NAME = "Cross Features" ;

	const uint32_t FEATURE_TERM_KEY = 1000; // term特征: key中的term

	const uint32_t FEATURE_SPAN_CATE = 2000000; // cate段内长度

	const uint32_t CROSS_FEATURE_NUM = 1;

	SFeatureInfo supportedFeature[] = 
    {
        {"TermCrossFeature"    , FEATURE_TERM_KEY 		     , 8000000, 9999999,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
    };
};

t_uint_uint_map CCrossFeature::match_term_map(300000);
t_uint_uint_map CCrossFeature::query_nonmatch_map(1000000);
t_uint_uint_map CCrossFeature::ad_nonmatch_map(300000);
Tree CCrossFeature::feature_tree(1000000);
uint32_t CCrossFeature::CATE_FEATURE_BASE = 30000000;
uint32_t CCrossFeature::CROSS_FEATURE_BASE = 8000000;

CCrossFeature::CCrossFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CCrossFeature::CCrossFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CCrossFeature::Initialize(const string &modelPath)
{   
	bool load_map = false;
    for (size_t i = 0; i < _featureList.size() && i < CROSS_FEATURE_NUM; i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
			load_map = true;
        }
    }

	if (load_map)
	{
		string match_file = GetFullPath(modelPath.c_str(), "match.idx");
		string query_nonmatch_file = GetFullPath(modelPath.c_str(), "query_nonmatch.idx");
		string ad_nonmatch_file = GetFullPath(modelPath.c_str(), "ad_nonmatch.idx");
		string cross_feature_file = GetFullPath(modelPath.c_str(), "cross_feature.bin");

		if (load_termid_map(match_file.c_str(), match_term_map) != 0)
		{
			return -1;
		}

		if (load_termid_map(query_nonmatch_file.c_str(), query_nonmatch_map) != 0)
		{
			return -1;
		}

		if (load_termid_map(ad_nonmatch_file.c_str(), ad_nonmatch_map) != 0)
		{
			return -1;
		}

		/*
		   if (load_feature_map(cross_feature_file.c_str(), feature_map) != 0)
		   {
		   return -1;
		   }
		   */
		if (load_feature_tree(cross_feature_file.c_str()) != 0)
		{
			return -1;
		}

		CRegDataCenter::Instance()->LoggingNormal("Match Map Size: %u", match_term_map.current_size());
		CRegDataCenter::Instance()->LoggingNormal("Query Non-Match Map Size: %u", query_nonmatch_map.current_size());
		CRegDataCenter::Instance()->LoggingNormal("Ad Non-match Map Size: %u", ad_nonmatch_map.current_size());
	}
    return 0;
}

int CCrossFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	map<uint32_t, uint32_t> ad_term_map; // 记录title中出现的词
	map<uint32_t, uint32_t> query_term_map; // 记录query中出现的词
	map<uint32_t, uint32_t> match_map;
	for (size_t j=0; j<oneCase.termInfo.originalTerms.size(); j++)
	{
		query_term_map[oneCase.termInfo.originalTerms[j]]++;
	}

	for (size_t j=0; j<oneCase.termInfo.keyTerms.size(); j++)
	{
		ad_term_map[oneCase.termInfo.keyTerms[j]]++;
		if (query_term_map.find(oneCase.termInfo.keyTerms[j]) != query_term_map.end())
		{
			match_map[oneCase.termInfo.keyTerms[j]] ++;
		}
	}

	for (size_t j=0; j<oneCase.termInfo.titleTerms.size(); j++)
	{
		ad_term_map[oneCase.termInfo.titleTerms[j]]++;
		if (query_term_map.find(oneCase.termInfo.titleTerms[j]) != query_term_map.end())
		{
			match_map[oneCase.termInfo.titleTerms[j]] ++;
		}
	}

	vector<uint32_t> feature_list;
	
	uint32_t feature_no;
	//Match Feature
	for (map<uint32_t, uint32_t>::const_iterator iter = match_map.begin();
			iter != match_map.end(); iter++)
	{
		if (match_term_map.find(iter->first, feature_no) != -1)
		{
			feature_list.push_back(feature_no);
		}
	}

	//Query NonMatch Feature
	for (map<uint32_t, uint32_t>::const_iterator iter = query_term_map.begin();
			iter != query_term_map.end(); iter++)
	{
		if (match_map.find(iter->first) == match_map.end() &&
				query_nonmatch_map.find(iter->first, feature_no) != -1)
		{
			feature_list.push_back(feature_no);
		}
	}

	//AD NonMatch Feature
	for (map<uint32_t, uint32_t>::const_iterator iter = ad_term_map.begin();
			iter != ad_term_map.end(); iter ++)
	{
		if (match_map.find(iter->first) == match_map.end() &&
				ad_nonmatch_map.find(iter->first, feature_no) != -1)
		{
			feature_list.push_back(feature_no);
		}
	}
	//Cate Feaure
	feature_list.push_back(CATE_FEATURE_BASE + oneCase.queryAdInfo.categoryHigh);
	sort (feature_list.begin(), feature_list.end());

	vector<uint32_t> result;
	feature_tree.generateFeature(feature_list, result);
	/*
	vector<uint32_t> result;
	generateFeature(feature_list, result);
	*/

	for (uint32_t i=0; i<result.size(); i++)
	{
		IdWeight feature;
		feature.id = result[i] + CROSS_FEATURE_BASE;
		feature.weight = 1.0;
		features.push_back(feature);
	}
	
	//CRegDataCenter::Instance()->LoggingNormal("%u", result.size());

    return 0;
}


int CCrossFeature::load_termid_map(const char* filename, t_uint_uint_map& termid_map)
{
	ifstream in(filename);
	if (!in)
	{
		CRegDataCenter::Instance()->LoggingNormal("Load termid Map Error: %s\n", filename);
		return -1;
	}
	termid_map.unbind_all();
	string line;
	while (getline(in, line))
	{
		char tokens[MAX_FIELD_NUM][MAX_STRING_LENGTH];
		int size = 0;
		parse_string(line.c_str(), "\t", tokens, &size);
		if (size < 0)
		{
			continue;
		}
		uint32_t termid = atoi(tokens[0]);
		uint32_t index = atoi(tokens[1]);
		termid_map.bind(termid, index);
	}
	in.close();

	return 0;
}

int CCrossFeature::load_feature_map(const char * filename, t_string_int_map& feature_map)
{
	ifstream in(filename);
	if (!in)
	{
		CRegDataCenter::Instance()->LoggingNormal("Load feature Map Error: %s\n", filename);
		return -1;
	}
	feature_map.unbind_all();
	string line;
	uint32_t count = 0;
	while (getline(in, line))
	{
		char tokens[MAX_FIELD_NUM][MAX_STRING_LENGTH];
		int size = 0;
		parse_string(line.c_str(), "\t", tokens, &size);
		if (size < 1)
		{
			continue;
		}

		vector<uint32_t> feature_list;
		for (int i=1; i<=size; i++)
		{
			uint32_t feature = atoi(tokens[i]);
			feature_list.push_back(feature);
		}

		sort(feature_list.begin(), feature_list.end());

		ostringstream oss;
		for (uint32_t i=0; i<feature_list.size(); i++)
		{
			oss << feature_list[i] << "\t";
		}
		uint32_t feature_no = CROSS_FEATURE_BASE + count;
		count ++;
		feature_map.bind(oss.str().c_str(), feature_no);
	}
	in.close();

	return 0;
}

int CCrossFeature::load_feature_tree(const char* filename)
{
	if(feature_tree.loadBinary(filename) != 0)
	{
		CRegDataCenter::Instance()->LoggingNormal("Load feature Tree Error: %s\n", filename);
		return -1;
	}
	return 0;
}
