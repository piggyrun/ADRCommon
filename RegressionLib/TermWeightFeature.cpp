#include <fstream>
#include "TermWeightFeature.h"
#include "TermWeight.h"

#define FEATURE_DEBUG

namespace
{
	const char * MODULE_NAME = "TermWeightFeatures";

	const uint32_t CORE_TERM_MATCH = 801;

	const float SUBJECT_WEIGHT = 0.55;
	const float COMMERCIAL_WEIGHT = 0.45;

	const float MATCH_TERM_WEIGHT_THRESHOLD = 0.2;

	const uint32_t QUERY_CORE_TERM_MATCH_KEY = 1;
	const uint32_t QUERY_CORE_TERM_MATCH_TITLE = 2;
	const uint32_t QUERY_CORE_TERM_MATCH_DESC = 3;
	const uint32_t KEY_CORE_TERM_MATCH_QUERY = 4;
	const uint32_t TITLE_CORE_TERM_MATCH_QUERY = 5;
	const uint32_t DESC_CORE_TERM_MATCH_QUERY = 6;

	const uint32_t MATCH_FEILD_NUM = 6;
	
	SFeatureInfo supportedFeature[] = 
	{
		{"CoreTermMatch"   ,CORE_TERM_MATCH   ,70000   ,70700   ,IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE}
	};
};

CTermWeightFeature::CTermWeightFeature(bool isOnline, uint32_t implementation)
	:CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{	
}

CTermWeightFeature::CTermWeightFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
	:CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CTermWeightFeature::Initialize(const string &modelPath)
{
	bool load_data = false;
	for (size_t i = 0; i < _featureList.size(); i++)
	{
		if (_featureList[i].isUse == FEATURE_ENABLE)
		{
			load_data = true;
			break;
		}
	}
	if (load_data)
	{
		CTermWeight::Instance()->Initialize(modelPath);
	}
	return 0;
}

int CTermWeightFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	switch (featureInfo.featureId)
	{
	case CORE_TERM_MATCH:
		GetCoreTermMatch(featureInfo, oneCase, features);
		break;
	}
		
	return 0;
}

void CTermWeightFeature::GetCoreTermMatch(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	IdWeight feature;
	uint32_t max_weight[MATCH_FEILD_NUM];
	uint32_t weight_sum[MATCH_FEILD_NUM];
	bool is_most_core_term_match[MATCH_FEILD_NUM];
	uint32_t core_term_num[MATCH_FEILD_NUM];
	uint32_t core_term_match_num[MATCH_FEILD_NUM];
	uint32_t match_term_weight_sum[MATCH_FEILD_NUM];
	uint32_t match_term_weight_coverage[MATCH_FEILD_NUM];
	for (uint32_t i = 0; i < MATCH_FEILD_NUM; i++)
	{
		max_weight[i] = 0;
		weight_sum[i] = 0;
		is_most_core_term_match[i] = false;
		core_term_num[i] = 0;
		core_term_match_num[i] = 0;
		match_term_weight_sum[i] = 0;
		match_term_weight_coverage[i] = 0;
	}
	map<uint32_t, uint32_t> queryTerm2Weight;
	for (size_t j = 0; j < oneCase.termInfo.originalTerms.size(); j++)
	{
		uint32_t queryTermid = oneCase.termInfo.originalTerms[j];
		uint32_t queryTermWeight = CTermWeight::Instance()->GetMixWeight(queryTermid, SUBJECT_WEIGHT, COMMERCIAL_WEIGHT);
		queryTerm2Weight[queryTermid] += queryTermWeight;
		weight_sum[0] += queryTermWeight;
	//	fprintf(stderr, "query term id: %u, query term weight: %u\n", queryTermid, queryTerm2Weight[queryTermid]);
		if (max_weight[0] < queryTerm2Weight[queryTermid]) 
		{
			max_weight[0] = queryTerm2Weight[queryTermid];
		}
	}
	weight_sum[1] = weight_sum[0];
	weight_sum[2] = weight_sum[0];
	max_weight[1] = max_weight[0];
	max_weight[2] = max_weight[0];

	map<uint32_t, uint32_t> keyTerm2Weight;
	for (size_t j = 0; j < oneCase.termInfo.keyTerms.size(); j++)
	{
		uint32_t keyTermid = oneCase.termInfo.keyTerms[j];
		uint32_t keyTermWeight = CTermWeight::Instance()->GetMixWeight(keyTermid, SUBJECT_WEIGHT, COMMERCIAL_WEIGHT);
		keyTerm2Weight[keyTermid] += keyTermWeight;
		weight_sum[3] += keyTermWeight;
	//	fprintf(stderr, "key term id: %u, key term weight: %u\n", keyTermid, keyTerm2Weight[keyTermid]);
		if (max_weight[3] < keyTerm2Weight[keyTermid])
		{
			max_weight[3] = keyTerm2Weight[keyTermid];
		}
	}

	map<uint32_t, uint32_t> titleTerm2Weight;
	for (size_t j = 0; j < oneCase.termInfo.titleTerms.size(); j++)
	{
		uint32_t titleTermid = oneCase.termInfo.titleTerms[j];
		uint32_t titleTermWeight = CTermWeight::Instance()->GetMixWeight(titleTermid, SUBJECT_WEIGHT, COMMERCIAL_WEIGHT);
		titleTerm2Weight[titleTermid] += titleTermWeight;
		weight_sum[4] += titleTermWeight;
	//	fprintf(stderr, "title term id: %u, title term weight: %u\n", titleTermid, titleTerm2Weight[titleTermid]);
		if (max_weight[4] < titleTerm2Weight[titleTermid])
		{
			max_weight[4] = titleTerm2Weight[titleTermid];
		}
	}

	map<uint32_t, uint32_t> descTerm2Weight;
	for (size_t j = 0; j < oneCase.termInfo.descTerms.size(); j++)
	{
		uint32_t descTermid = oneCase.termInfo.descTerms[j];
		uint32_t descTermWeight = CTermWeight::Instance()->GetMixWeight(descTermid, SUBJECT_WEIGHT, COMMERCIAL_WEIGHT);
		descTerm2Weight[descTermid] += descTermWeight;
		weight_sum[5] += descTermWeight;
	//	fprintf(stderr, "desc term id: %u, desc term weight: %u\n", descTermid, descTerm2Weight[descTermid]);
		if (max_weight[5] < descTerm2Weight[descTermid])
		{
			max_weight[5] = descTerm2Weight[descTermid];
		}
	}

	for (map<uint32_t, uint32_t>::iterator iter = queryTerm2Weight.begin(); iter != queryTerm2Weight.end(); ++iter)
	{
		if (keyTerm2Weight.find(iter->first) != keyTerm2Weight.end())
		{
			match_term_weight_sum[0] += iter->second;
			if ((max_weight[0] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[0])
			{
				core_term_match_num[0]++;
				if (iter->second == max_weight[0])
				{
					is_most_core_term_match[0] = true;
				}
			}
		}
			
		if (titleTerm2Weight.find(iter->first) != titleTerm2Weight.end())
		{
			match_term_weight_sum[1] += iter->second;
			if ((max_weight[1] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[1])
			{
				core_term_match_num[1]++;
				if (iter->second == max_weight[1])
				{
					is_most_core_term_match[1] = true;
				}
			}
		}

		if (descTerm2Weight.find(iter->first) != descTerm2Weight.end())
		{
			match_term_weight_sum[2] += iter->second;
			if ((max_weight[2] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[2])
			{
				core_term_match_num[2]++;
				if (iter->second == max_weight[2])
				{
					is_most_core_term_match[2] = true;
				}
			}
		}

		for (uint32_t k = 0; k < 3; k++)
		{
			if ((max_weight[k] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[k])
			{
				core_term_num[k]++;
			}
		}
	}

	for (map<uint32_t, uint32_t>::iterator iter = keyTerm2Weight.begin(); iter != keyTerm2Weight.end(); ++iter)
	{
		if (queryTerm2Weight.find(iter->first) != queryTerm2Weight.end())
		{
			match_term_weight_sum[3] += iter->second;
			if ((max_weight[3] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[3])
			{
				core_term_match_num[3]++;
				if (iter->second == max_weight[3])
				{
					is_most_core_term_match[3] = true;
				}
			}
		}
		if ((max_weight[3] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[3])
		{
			core_term_num[3]++;
		}
	}

	for (map<uint32_t, uint32_t>::iterator iter = titleTerm2Weight.begin(); iter != titleTerm2Weight.end(); ++iter)
	{
		if (queryTerm2Weight.find(iter->first) != queryTerm2Weight.end())
		{
			match_term_weight_sum[4] += iter->second;
			if ((max_weight[4] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[4])
			{
				core_term_match_num[4]++;
				if (iter->second == max_weight[4])
				{
					is_most_core_term_match[4] = true;
				}
			}
		}
		if ((max_weight[4] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[4])
		{
			core_term_num[4]++;
		}
	}

	for (map<uint32_t, uint32_t>::iterator iter = descTerm2Weight.begin(); iter != descTerm2Weight.end(); ++iter)
	{
		if (queryTerm2Weight.find(iter->first) != queryTerm2Weight.end())
		{
			match_term_weight_sum[5] += iter->second;
			if ((max_weight[5] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[5])
			{
				core_term_match_num[5]++;
				if (iter->second == max_weight[5])
				{
					is_most_core_term_match[5] = true;
				}
			}
		}
		if ((max_weight[5] - iter->second) < MATCH_TERM_WEIGHT_THRESHOLD * max_weight[5])
		{
			core_term_num[5]++;
		}
	}
	for (uint32_t i = 0; i < MATCH_FEILD_NUM; i++)
	{
		if (is_most_core_term_match[i])
		{
			feature.id = featureInfo.startOffset + i;
			feature.weight = 1.0;
			features.push_back(feature);
		}
		if (core_term_match_num[i] > 3)
		{
			core_term_match_num[i] = 3;
		}
		if (core_term_num[i] > 3)
		{
			core_term_num[i] = 3;
		}
		feature.id = featureInfo.startOffset + 10 * (i + 1) + core_term_num[i]*(core_term_num[i]+1)/2 + core_term_match_num[i];
		feature.weight = 1.0;
		features.push_back(feature);

		if (weight_sum[i] <= 0)
			weight_sum[i] = 1;
		match_term_weight_coverage[i] = match_term_weight_sum[i] * 100 / weight_sum[i];
		if (match_term_weight_coverage[i] == 100)
		{
			match_term_weight_coverage[i] = 99;
		}

		feature.id = featureInfo.startOffset + 100 * (i + 1) + match_term_weight_coverage[i];
		feature.weight = 1.0;
		features.push_back(feature);

	//	fprintf(stderr, "is_most_core_term_match[%u]: %u\n", i, is_most_core_term_match[i]);
	//	fprintf(stderr, "core_term_num[%u]: %u\tcore_term_match_num[%u]: %u\n", i, core_term_num[i], i, core_term_match_num[i]);
	//	fprintf(stderr, "weight_sum[%u]: %u\tmatch_term_weight_sum[%u]: %u\tmatch_term_weight_coverage[%u]: %u\n", i, weight_sum[i], i, match_term_weight_sum[i], i, match_term_weight_coverage[i]);
	}
}

