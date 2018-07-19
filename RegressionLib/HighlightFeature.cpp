#include <fstream>
#include "HighlightFeature.h"

namespace
{
    const char * MODULE_NAME = "Highlight Features" ;

    const uint32_t FEATURE_HIGHLIGHT      		 = 500;
    const uint32_t FEATURE_HIGHLIGHT_QT   		 = 501;
    const uint32_t FEATURE_MATCH_COVER_QT        = 502;

	const uint32_t MAX_COL_NUM = 20;
	const uint32_t MAX_TITLE_LEN = 100;
	const uint32_t FEATURE_OFFSET_MATCH_COVER = 400;

    SFeatureInfo supportedFeature[] = 
    {
        {"Highlight"     	, FEATURE_HIGHLIGHT      	 , 50000, 50099,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"HighlightQT"   	, FEATURE_HIGHLIGHT_QT       , 50100, 50499,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_DISABLE},
        {"MatchCoverQT"     , FEATURE_MATCH_COVER_QT     , 50500, 51299,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE}
    };
};

CHighlightFeature::CHighlightFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CHighlightFeature::CHighlightFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CHighlightFeature::Initialize(const string &modelPath)
{   
    return 0;
}

int CHighlightFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{

    switch (featureInfo.featureId)
    {
    case FEATURE_HIGHLIGHT:
        GetHighlight(featureInfo, oneCase, features);
        break;
	case FEATURE_HIGHLIGHT_QT:
		GetHighlightQT(featureInfo, oneCase, features);
		break;
	case FEATURE_MATCH_COVER_QT:
		GetMatchCoverQT(featureInfo, oneCase, features);
		break;
    }
    return 0;
}

void CHighlightFeature::GetHighlight(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	if (oneCase.queryDocInfo.title.empty())
		return;
	// 设title字符串最长为100，0~99其中能标红的位，取1，否则取0
	IdWeight feature;
	size_t pos = 0;
	bool is_red = false;
	string title(oneCase.queryDocInfo.title);
	size_t titlelen = title.size() > MAX_TITLE_LEN ? MAX_TITLE_LEN : title.size();
	for (size_t i=0; i<titlelen-1;)
	{
		if ((unsigned char)title[i] == 0xFD) // highlight sign
		{
			if ((unsigned char)title[i+1] == 0xA1)
			{
				is_red = true;
				i += 2;
				continue;
			}
			if ((unsigned char)title[i+1] == 0xA2)
			{
				is_red = false;
				i += 2;
				continue;
			}
		}
		if (is_red)
		{
			feature.id = featureInfo.startOffset + pos;
			feature.weight = 1.0;
			features.push_back(feature);
#ifdef FEATURE_DEBUG
			LogFeatureInfo(featureInfo, feature, "red: %u", pos);
#endif
		}
		pos++;
		i++;
	}
}

void CHighlightFeature::GetHighlightQT(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	IdWeight feature;
	/////////////// highlight特征 /////////////
	uint32_t highlight_array[MAX_COL_NUM][MAX_COL_NUM];
	// 位置上
	memset(highlight_array, 0, sizeof(uint32_t)*MAX_COL_NUM*MAX_COL_NUM);
	for (size_t row=0; row < oneCase.termInfo.originalTerms.size(); row++)
	{
		for (size_t col=0; col < oneCase.termInfo.titleTerms.size(); col++)
		{
			if (oneCase.termInfo.originalTerms[row] == oneCase.termInfo.titleTerms[col])
			{
				size_t r = row >= MAX_COL_NUM ? MAX_COL_NUM-1 : row;
				size_t c = col >= MAX_COL_NUM ? MAX_COL_NUM-1 : col;
				highlight_array[r][c] = 1;
			}
		}
	}
	size_t title_highlight_num = 0;
	for (size_t col=0; col<MAX_COL_NUM; col++)
	{
		size_t hit_num = 0; // 按title的term，query多次，只算一次
		for (size_t row=0; row<MAX_COL_NUM; row++)
		{
			if (highlight_array[row][col] == 1)
			{
				feature.id = featureInfo.startOffset + row*MAX_COL_NUM + col;
				feature.weight = 1.0;
				features.push_back(feature);
#ifdef FEATURE_DEBUG
				LogFeatureInfo(featureInfo, feature, "");
#endif
				hit_num++;
			}
		}
		if (hit_num > 0)
			title_highlight_num++; // 标题标红词的数量
	}
	/*/ 数量上
	size_t title_term_num = oneCase.termInfo.titleTerms.size() - 1;
	if (title_term_num >= MAX_COL_NUM)
		title_term_num = MAX_COL_NUM - 1;
	if (title_highlight_num >= MAX_COL_NUM)
		title_highlight_num = MAX_COL_NUM - 1;
	feature.id = featureInfo.startOffset + FEATURE_OFFSET_HIGHLIGHT_NUM + title_highlight_num*MAX_COL_NUM + title_term_num;
	feature.weight = 1.0;
	features.push_back(feature);
#ifdef FEATURE_DEBUG
	LogFeatureInfo(featureInfo, feature, "");
#endif
	//*/
}

void CHighlightFeature::GetMatchCoverQT(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	IdWeight feature;

	map<uint32_t, uint32_t> tterm_map; // 记录title中出现的词
	map<uint32_t, uint32_t> qterm_map; // 记录query中出现的词
	// 位置上
	for (size_t row=0; row < oneCase.termInfo.originalTerms.size(); row++)
	{
		qterm_map[oneCase.termInfo.originalTerms[row]]++;
	}
	for (size_t col=0; col < oneCase.termInfo.titleTerms.size(); col++)
	{
		tterm_map[oneCase.termInfo.titleTerms[col]]++;
	}
	/////////////// match cover特征 /////////////
	// match cover: query_hit/query
	size_t query_term_num = oneCase.termInfo.originalTerms.size();
	if (query_term_num >= MAX_COL_NUM)
		query_term_num = MAX_COL_NUM - 1;
	size_t hit_query_term_num = 0;
	for (size_t j=0; j < oneCase.termInfo.originalTerms.size(); j++)
	{
		uint32_t termid = oneCase.termInfo.originalTerms[j];
		if (tterm_map.find(termid) != tterm_map.end())
			hit_query_term_num++;
	}
	if (hit_query_term_num >= MAX_COL_NUM)
		hit_query_term_num = MAX_COL_NUM - 1;
	feature.id = featureInfo.startOffset + hit_query_term_num*MAX_COL_NUM + query_term_num;
	feature.weight = 1.0;
	features.push_back(feature);
#ifdef FEATURE_DEBUG
	LogFeatureInfo(featureInfo, feature, "query_hit:%u, query_num:%u", hit_query_term_num, query_term_num);
#endif
	// match cover: title_hit/title
	size_t title_term_num = oneCase.termInfo.titleTerms.size();
	if (title_term_num >= MAX_COL_NUM)
		title_term_num = MAX_COL_NUM - 1;
	size_t hit_title_term_num = 0;
	for (size_t j=0; j < oneCase.termInfo.titleTerms.size(); j++)
	{
		uint32_t termid = oneCase.termInfo.titleTerms[j];
		if (qterm_map.find(termid) != qterm_map.end())
			hit_title_term_num++;
	}
	if (hit_title_term_num >= MAX_COL_NUM)
		hit_title_term_num = MAX_COL_NUM - 1;
	feature.id = featureInfo.startOffset + FEATURE_OFFSET_MATCH_COVER + hit_title_term_num*MAX_COL_NUM + title_term_num;
	feature.weight = 1.0;
	features.push_back(feature);
#ifdef FEATURE_DEBUG
	LogFeatureInfo(featureInfo, feature, "title_hit:%u, title_num:%u", hit_title_term_num, title_term_num);
#endif
}
