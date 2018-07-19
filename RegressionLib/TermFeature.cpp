#include <fstream>
#include "TermFeature.h"

namespace
{
    const char * MODULE_NAME = "Term Features" ;

	const uint32_t FEATURE_TERM_KEY = 601; // term特征: key中的term
	const uint32_t FEATURE_TERM_TITLE = 602; // term特征: title中的term
	const uint32_t FEATURE_TERM_QUERY = 603; // term特征: query中的term
	const uint32_t FEATURE_TERM_QUERY_KEY = 604; // term特征：query与key(cate)同现的
	const uint32_t FEATURE_TERM_QUERY_TITLE = 605; // term特征：query与title(cate)同现的

	const uint32_t FEATURE_TERM_KEY_HIT = 606;
	const uint32_t FEATURE_TERM_TITLE_HIT = 607;

	const uint32_t FEATURE_TERM_MATCH_QK = 608; // query与key的匹配最长串
	const uint32_t FEATURE_TERM_MATCH_QT = 609; // query与title的匹配最长串
	const uint32_t FEATURE_TERM_MATCH_NON_QK = 610; // query中匹配与非匹配(与key)的两两组合
	const uint32_t FEATURE_TERM_MATCH_NON_QT = 611; // query中匹配与非匹配(与title)的两两组合

	const uint32_t FEATURE_SPAN_CATE = 50000; // cate段内长度

	const uint32_t TERM_FEATURE_DIC_NUM = 5;
	const uint32_t TERM_HIT_FEATURE_NUM = 2;
	const uint32_t TERM_MATCH_FEATURE_NUM = 4;
	const uint32_t TERM_TOTAL_FEATURE_NUM = TERM_FEATURE_DIC_NUM + TERM_HIT_FEATURE_NUM + TERM_MATCH_FEATURE_NUM;

	const uint32_t POS_SHIFT = 900000; // 起始移到1000000

	SFeatureInfo supportedFeature[] = 
    {
        {"TermQueryKeyMiss"    , FEATURE_TERM_KEY 		     , 100000+POS_SHIFT, 219999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermQueryTitleMiss"  , FEATURE_TERM_TITLE 	     , 220000+POS_SHIFT, 339999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermQuery"           , FEATURE_TERM_QUERY    		 , 340000+POS_SHIFT, 459999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermQueryKeyHit"     , FEATURE_TERM_QUERY_KEY      , 460000+POS_SHIFT, 1259999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermQueryTitleHit"   , FEATURE_TERM_QUERY_TITLE    , 1260000+POS_SHIFT, 2059999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},

        {"TermKeyHitCombine"   , FEATURE_TERM_KEY_HIT        , 90000, 90099,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermTitleHitCombine" , FEATURE_TERM_TITLE_HIT      , 90100, 90199,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},

        {"TermMatchQK"   	   , FEATURE_TERM_MATCH_QK       , 2060000+POS_SHIFT, 2259999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermMatchQT"   	   , FEATURE_TERM_MATCH_QT       , 2260000+POS_SHIFT, 2459999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermMatchNonMatchQK" , FEATURE_TERM_MATCH_NON_QK   , 2460000+POS_SHIFT, 4459999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"TermMatchNonMatchQT" , FEATURE_TERM_MATCH_NON_QT   , 4460000+POS_SHIFT, 6459999+POS_SHIFT,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE}
    };
};

t_termid_fnum_map CTermFeature::key_termid_fnum_map(300000);
t_termid_fnum_map CTermFeature::title_termid_fnum_map(300000);
t_termid_fnum_map CTermFeature::query_termid_fnum_map(300000);
t_termid_fnum_map CTermFeature::query_key_termid_fnum_map(100000);
t_termid_fnum_map CTermFeature::query_title_termid_fnum_map(100000);

t_uint_uint_map CTermFeature::match_query_title_map(300000);
t_uint_uint_map CTermFeature::match_non_query_title_map(2000000);

CTermFeature::CTermFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CTermFeature::CTermFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CTermFeature::Initialize(const string &modelPath)
{   
    vector<uint32_t> isEnable(TERM_TOTAL_FEATURE_NUM);
    for (size_t i = 0; i < _featureList.size() && i < TERM_TOTAL_FEATURE_NUM; i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            isEnable[i] = 1;
        }
    }
	// term特征
	string key_termid_file = GetFullPath(modelPath.c_str(), "key_term_dictionary");
	string title_termid_file = GetFullPath(modelPath.c_str(), "title_term_dictionary");
	string query_termid_file = GetFullPath(modelPath.c_str(), "query_term_dictionary");
	string query_key_termid_file = GetFullPath(modelPath.c_str(), "query_key_term_dictionary");
	string query_title_termid_file = GetFullPath(modelPath.c_str(), "query_title_term_dictionary");
	t_termid_fnum_map * termid_map[TERM_FEATURE_DIC_NUM] = {
		&key_termid_fnum_map, &title_termid_fnum_map, &query_termid_fnum_map, &query_key_termid_fnum_map, &query_title_termid_fnum_map};
	string filename[TERM_FEATURE_DIC_NUM] = {
		key_termid_file, title_termid_file, query_termid_file, query_key_termid_file, query_title_termid_file};
    for (uint32_t i = 0; i < TERM_FEATURE_DIC_NUM; i++)
    {
        if (isEnable[i] == 1)
        {
			if (load_termid_map(filename[i].c_str(), *termid_map[i]) != 0) return -1;
        }
    }
	// match特征
	string match_query_title_file = GetFullPath(modelPath.c_str(), "match_query_1_title_1");
	string match_non_query_title_file = GetFullPath(modelPath.c_str(), "match_query_1_title_0");
	if (isEnable[TERM_FEATURE_DIC_NUM+TERM_HIT_FEATURE_NUM] == 1 || isEnable[TERM_FEATURE_DIC_NUM+TERM_HIT_FEATURE_NUM+1] == 1)
	{
		if (load_match_term_map(match_query_title_file.c_str(), match_query_title_map) != 0) return -1;
	}
	if (isEnable[TERM_FEATURE_DIC_NUM+TERM_HIT_FEATURE_NUM+2] == 1 || isEnable[TERM_FEATURE_DIC_NUM+TERM_HIT_FEATURE_NUM+3] == 1)
	{
		if (load_match_term_map(match_non_query_title_file.c_str(), match_non_query_title_map) != 0) return -1;
	}

    return 0;
}

int CTermFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    return 0;
}

int CTermFeature::GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	map<uint32_t, uint32_t> kterm_map; // 记录key中出现的词
	map<uint32_t, uint32_t> tterm_map; // 记录title中出现的词
	map<uint32_t, uint32_t> qterm_map; // 记录query中出现的词
	for (size_t j=0; j<oneCase.termInfo.keyTerms.size(); j++)
	{
		kterm_map[oneCase.termInfo.keyTerms[j]]++;
	}
	for (size_t j=0; j<oneCase.termInfo.titleTerms.size(); j++)
	{
		tterm_map[oneCase.termInfo.titleTerms[j]]++;
	}
	for (size_t j=0; j<oneCase.termInfo.originalTerms.size(); j++)
	{
		qterm_map[oneCase.termInfo.originalTerms[j]]++;
	}

    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
			vector<IdWeight> feature_vec;
			GetTermFeature(_featureList[i], oneCase, kterm_map, tterm_map, qterm_map, feature_vec);
			for (size_t n=0; n<feature_vec.size(); n++)
				features.push_back(feature_vec[n]);
		}
	}

    return 0;
}

void CTermFeature::GetTermFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, 
		const map<uint32_t, uint32_t> &kterm_map,
		const map<uint32_t, uint32_t> &tterm_map,
		const map<uint32_t, uint32_t> &qterm_map,
		vector<IdWeight> &feature_vec) const
{
	map<uint32_t , uint32_t>::const_iterator it;
	map<uint32_t , uint32_t>::const_iterator it2;
	uint32_t cate_index = oneCase.queryAdInfo.categoryHigh;
	size_t hit_combine_key = 0;
	size_t hit_combine_title = 0;
	IdWeight feature;
	uint32_t feature_index = 0;
	vector<uint32_t> feature_index_vec;
	string debug_str;
	vector<string> debug_str_vec;

	switch (featureInfo.featureId)
	{
		case FEATURE_TERM_QUERY:
			// only query
			for (it=qterm_map.begin(); it!=qterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				if (tterm_map.find(termid) == tterm_map.end() 
						&& kterm_map.find(termid) == kterm_map.end()
						&& query_termid_fnum_map.find(termid, index) != -1) // query有而title中没有的term
				{
					feature.id = featureInfo.startOffset + index;
					feature.weight = 1.0;
					feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
					LogFeatureInfo(featureInfo, feature, "%u", termid);
#endif
				}
			}
			break;
		case FEATURE_TERM_QUERY_KEY:
			for (it=kterm_map.begin(); it!=kterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				if (qterm_map.find(termid) != qterm_map.end()) // 共有的term
				{
					if (query_key_termid_fnum_map.find(termid, index) != -1)
					{
						feature.id = featureInfo.startOffset + FEATURE_SPAN_CATE*(cate_index-1) + index;
						feature.weight = 1.0;
						feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
						LogFeatureInfo(featureInfo, feature, "%u", termid);
#endif
					}
				}
			}
			break;
		case FEATURE_TERM_KEY:
			for (it=kterm_map.begin(); it!=kterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				if (qterm_map.find(termid) == qterm_map.end()) // 非共有的term
				{
					if (key_termid_fnum_map.find(termid, index) != -1)
					{
						feature.id = featureInfo.startOffset + index;
						feature.weight = 1.0;
						feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
						LogFeatureInfo(featureInfo, feature, "%u", termid);
#endif
					}
				}
			}
			break;
		case FEATURE_TERM_QUERY_TITLE:
			for (it=tterm_map.begin(); it!=tterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				if (qterm_map.find(termid) != qterm_map.end()) // 共有的term
				{
					if (query_title_termid_fnum_map.find(termid, index) != -1)
					{
						feature.id = featureInfo.startOffset + FEATURE_SPAN_CATE*(cate_index-1) + index;
						feature.weight = 1.0;
						feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
						LogFeatureInfo(featureInfo, feature, "%u", termid);
#endif
					}
				}
			}
			break;
		case FEATURE_TERM_TITLE:
			for (it=tterm_map.begin(); it!=tterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				if (qterm_map.find(termid) == qterm_map.end()) // 非共有的term
				{
					if (title_termid_fnum_map.find(termid, index) != -1)
					{
						feature.id = featureInfo.startOffset + index;
						feature.weight = 1.0;
						feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
						LogFeatureInfo(featureInfo, feature, "%u", termid);
#endif
					}
				}
			}
			break;
		case FEATURE_TERM_KEY_HIT:
			for (it=kterm_map.begin(); it!=kterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				it2 = qterm_map.find(termid);
				if (it2 != qterm_map.end()) // 共有的term
				{
					if (query_key_termid_fnum_map.find(termid, index) != -1)
					{
						// 命中数组合特征
						hit_combine_key += (it2->second * it->second);
					}
				}
			}
			feature.id = featureInfo.startOffset + hit_combine_key;
			feature.weight = 1.0;
			feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
			LogFeatureInfo(featureInfo, feature, "%u", hit_combine_key);
#endif
			break;
		case FEATURE_TERM_TITLE_HIT:
			for (it=tterm_map.begin(); it!=tterm_map.end(); ++it)
			{
				uint32_t termid, index;
				termid = it->first;
				it2 = qterm_map.find(termid);
				if (it2 != qterm_map.end()) // 共有的term
				{
					if (query_title_termid_fnum_map.find(termid, index) != -1)
					{
						// 命中数组合特征
						hit_combine_title += (it2->second * it->second);
					}
				}
			}
			feature.id = featureInfo.startOffset + hit_combine_title;
			feature.weight = 1.0;
			feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
			LogFeatureInfo(featureInfo, feature, "%u", hit_combine_title);
#endif
			break;
		case FEATURE_TERM_MATCH_QK:
			if (get_match_term_feature(qterm_map, kterm_map, match_query_title_map, feature_index, debug_str) == 0)
			{
				feature.id = featureInfo.startOffset + feature_index;
				feature.weight = 1.0;
				feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
				LogFeatureInfo(featureInfo, feature, "%s\t%u", debug_str.c_str(), feature_index);
#endif
			}
			break;
		case FEATURE_TERM_MATCH_QT:
			if (get_match_term_feature(qterm_map, tterm_map, match_query_title_map, feature_index, debug_str) == 0)
			{
				feature.id = featureInfo.startOffset + feature_index;
				feature.weight = 1.0;
				feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
				LogFeatureInfo(featureInfo, feature, "%s\t%u", debug_str.c_str(), feature_index);
#endif
			}
			break;
		case FEATURE_TERM_MATCH_NON_QK:
			get_match_non_term_feature(qterm_map, kterm_map, match_non_query_title_map, feature_index_vec, debug_str_vec);
			for (size_t n=0; n<feature_index_vec.size(); n++)
			{
				feature.id = featureInfo.startOffset + feature_index_vec[n];
				feature.weight = 1.0;
				feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
				LogFeatureInfo(featureInfo, feature, "%s\t%u", debug_str_vec[n].c_str(), feature_index_vec[n]);
#endif
			}
			break;
		case FEATURE_TERM_MATCH_NON_QT:
			get_match_non_term_feature(qterm_map, tterm_map, match_non_query_title_map, feature_index_vec, debug_str_vec);
			for (size_t n=0; n<feature_index_vec.size(); n++)
			{
				feature.id = featureInfo.startOffset + feature_index_vec[n];
				feature.weight = 1.0;
				feature_vec.push_back(feature);
#ifdef FEATURE_DEBUG
				LogFeatureInfo(featureInfo, feature, "%s\t%u", debug_str_vec[n].c_str(), feature_index_vec[n]);
#endif
			}
			break;
		default:
			break;

	}
}

int CTermFeature::load_termid_map(const char * filename, t_termid_fnum_map &termid_map)
{
	ifstream in(filename);
	if (!in)
	{
		CRegDataCenter::Instance()->LoggingNormal("[regression model termid feature] open %s failed ]", filename);
		return -1;
	}
	termid_map.unbind_all();
	string line;
	while (getline(in, line))
	{
		char tokens[MAX_FIELD_NUM][MAX_STRING_LENGTH];
		int size = 0;
		parse_string(line.c_str(), "\t", tokens, &size);
		if (size < 1)
		{
			CRegDataCenter::Instance()->LoggingNormal("[regression model termid feature fields missing !!! ]");
			continue;
		}
		uint32_t termid = atoi(tokens[1]);
		uint32_t index = atoi(tokens[0]);
		termid_map.bind(termid, index);
	}
	in.close();
	CRegDataCenter::Instance()->LoggingNormal("[Size of (%s) termid_map:  %u ]", filename, (uint32_t)termid_map.current_size());

	return 0;
}

int CTermFeature::load_match_term_map(const char * filename, t_uint_uint_map &match_map)
{
	ifstream in(filename);
	if (!in)
	{
		CRegDataCenter::Instance()->LoggingNormal("[regression model match term feature] open %s failed ]", filename);
		return -1;
	}
	match_map.unbind_all();
	string line;
	uint32_t index = 0;
	while (getline(in, line))
	{
		vector<string> tokens;
		SplitString(line, '\t', tokens);
		if (tokens.size() < 2)
		{
			CRegDataCenter::Instance()->LoggingNormal("[regression model termid feature fields missing !!! ]");
			continue;
		}
		md5_long_32 md5_output = getSign32(tokens[1].c_str(), -1);
		uint32_t key = md5_output.data.intData[0];
		match_map.bind(key, index);
		index++;
	}
	in.close();
	CRegDataCenter::Instance()->LoggingNormal("[Size of (%s) match_term_map:  %u ]", filename, (uint32_t)match_map.current_size());

	return 0;
}

int CTermFeature::get_match_term_feature(const map<uint32_t, uint32_t> &qterm_map, const map<uint32_t, uint32_t> &term_map, const t_uint_uint_map &match_map, uint32_t &feature_index, string &debug_str) const
{
	map<uint32_t, uint32_t>::const_iterator it;
	vector<uint32_t> match_term_list;
	match_term_list.clear();
	bool is_success = false;
	for (it=qterm_map.begin(); it!=qterm_map.end(); ++it)
	{
		if (term_map.find(it->first) != term_map.end())
			match_term_list.push_back(it->first);
	}
	if (match_term_list.size() > 1) // 至少得两个词
	{
		sort(match_term_list.begin(), match_term_list.end());
		ostringstream oss;
		oss << (uint32_t)match_term_list[0];
		for (size_t i = 1; i < match_term_list.size(); i++)
		{
			oss << "" << (uint32_t)match_term_list[i];
		}
		md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
		debug_str = oss.str().c_str();
		uint32_t key = md5_output.data.intData[0];
		uint32_t index;
		if (match_map.find(key, index) != -1)
		{
			feature_index = index;
			is_success = true;
		}
	}
	if (is_success)
		return 0; // 成功
	else
		return -1;
}

int CTermFeature::get_match_non_term_feature(const map<uint32_t, uint32_t> &qterm_map, const map<uint32_t, uint32_t> &term_map, const t_uint_uint_map &match_map, vector<uint32_t> &feature_index_vec, vector<string> &debug_str_vec) const
{
	map<uint32_t, uint32_t>::const_iterator it;
	vector<uint32_t> match_term_list, non_match_term_list;
	match_term_list.clear();
	non_match_term_list.clear();
	feature_index_vec.clear();
	debug_str_vec.clear();
	for (it=qterm_map.begin(); it!=qterm_map.end(); ++it)
	{
		if (term_map.find(it->first) != term_map.end())
			match_term_list.push_back(it->first);
		else
			non_match_term_list.push_back(it->first);
	}
	// 两两配对
	for (size_t i=0; i<match_term_list.size(); i++)
	{
		for (size_t j=0; j<non_match_term_list.size(); j++)
		{
			ostringstream oss;
			oss << (uint32_t)match_term_list[i] << "" << (uint32_t)non_match_term_list[j];
			md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
			uint32_t key = md5_output.data.intData[0];
			uint32_t index;
			if (match_map.find(key, index) != -1)
			{
				feature_index_vec.push_back(index);
				debug_str_vec.push_back(oss.str().c_str());
			}
		}
	}
	return 0;
}
