#include <fstream>
#include "SiteFeature.h"
#include "DataProcessor.h"
#include "Platform/docId/docId.h"

namespace
{
    const char * MODULE_NAME = "Site Features" ;

	const uint32_t FEATURE_BIG_SITE      = 700;
	const uint32_t FEATURE_SITE_RANK     = 701;

	const uint32_t BIG_SITE_BUCKET_SIZE = 10000;
	const uint32_t SITE_RANK_BUCKET_SIZE = 10000000;

    const char * BIG_SITE_FILE     = "big_site_list";
    const char * SITE_RANK_FILE    = "site_rank";

    SFeatureInfo supportedFeature[] = 
    {
        {"BigSite" 	   , FEATURE_BIG_SITE      , 15000, 18999, IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"SiteRank"	   , FEATURE_SITE_RANK     , 19000, 19199, IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE}
    };
};

CSiteFeature::CSiteFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CSiteFeature::CSiteFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CSiteFeature::Initialize(const string &modelPath)
{   
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            uint32_t result = 0;            
            switch (_featureList[i].featureId)
            {
            case FEATURE_BIG_SITE:
				result = LoadListMap(modelPath.c_str(), BIG_SITE_FILE, _bigSiteMap, BIG_SITE_BUCKET_SIZE, true);
                break;
			case FEATURE_SITE_RANK:
				result = LoadSiteRank(modelPath.c_str(), SITE_RANK_FILE, _siteRankMap, SITE_RANK_BUCKET_SIZE, false);
				break;
            }
            if (result != 0)
            {
                throw;
            }
        }
    }
    return 0;
}

void CSiteFeature::GetBigSite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	md5_long_32 md5_output = getSign32(oneCase.queryDocInfo.site.c_str(), -1);
	uint32_t key = md5_output.data.intData[0]; 
	uint32_t index = 0;
	if (_bigSiteMap.find(key, index) != -1)
	{
		IdWeight feature;
		feature.id = index + featureInfo.startOffset;
		feature.weight = 1.0;
		features.push_back(feature);
#ifdef FEATURE_DEBUG
		LogFeatureInfo(featureInfo, feature, "%s, index: %u", oneCase.queryDocInfo.site.c_str(), index);
#endif
	}
}

void CSiteFeature::GetSiteRank(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	gDocID_t docid;
	url2DocId(oneCase.queryDocInfo.url.c_str(), &docid);
	uint64_t siteid = docid.id.obs.host_id;
	uint32_t rank = 0;
	if (_siteRankMap.find(siteid, rank) == -1)
	{
		rank = 0;
	}
	IdWeight feature;
	feature.id = rank*100/65536+1 + featureInfo.startOffset;
	feature.weight = 1.0;
	features.push_back(feature);
#ifdef FEATURE_DEBUG
	LogFeatureInfo(featureInfo, feature, "%s, %llu, rank: %u", oneCase.queryDocInfo.site.c_str(), siteid, rank);
#endif
}

int CSiteFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    switch (featureInfo.featureId)
    {
	case FEATURE_BIG_SITE:
		GetBigSite(featureInfo, oneCase, features);
		break;
	case FEATURE_SITE_RANK:
		GetSiteRank(featureInfo, oneCase, features);
		break;
    }

    return 0;
}

