#include <fstream>
#include "ClickFeature.h"
#include "DataProcessor.h"

//#define FEATURE_DEBUG
namespace
{
    const char * MODULE_NAME = "Click Features" ;

    const uint32_t FEATURE_BAIDU_QUERY_URL      = 700;
    const uint32_t FEATURE_GOOGLE_QUERY_URL     = 701;

	const uint32_t BAIDU_QUERY_URL_BUCKET_SIZE = 30000000;
	const uint32_t GOOGLE_QUERY_URL_BUCKET_SIZE = 3000000;
    
    const char * BAIDU_QUERY_URL_FILE	   = "baidu_query_url";
    const char * GOOGLE_QUERY_URL_FILE	   = "google_query_url";

    SFeatureInfo supportedFeature[] = 
    {
        {"BaiduQueryUrl"	   	   , FEATURE_BAIDU_QUERY_URL      , 40000,  40199,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE},
        {"GoogleQueryUrl"	   	   , FEATURE_GOOGLE_QUERY_URL     , 40200,  40399,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE}
    };
};

CClickFeature::CClickFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CClickFeature::CClickFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CClickFeature::Initialize(const string &modelPath)
{  
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            uint32_t result = 0;            
            switch (_featureList[i].featureId)
            {
            case FEATURE_BAIDU_QUERY_URL:
                result = LoadCtrBinaryMap(modelPath.c_str(), BAIDU_QUERY_URL_FILE, _baiduQueryUrlMap, BAIDU_QUERY_URL_BUCKET_SIZE, true);
                break;
            case FEATURE_GOOGLE_QUERY_URL:
                result = LoadCtrBinaryMap(modelPath.c_str(), GOOGLE_QUERY_URL_FILE, _googleQueryUrlMap, GOOGLE_QUERY_URL_BUCKET_SIZE, true);
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

void CClickFeature::GetQueryUrl(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, const Int2IntHash &data_map, vector<IdWeight> &features) const
{
	IdWeight feature;
	ostringstream oss; 
	oss << oneCase.queryDocInfo.query << '\3' << oneCase.queryDocInfo.url;
	md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
	uint32_t key = md5_output.data.intData[0];
	uint32_t offset = 0;
	if (data_map.find(key, offset) == -1)
	{
		offset = 0;
	}
	feature.id = featureInfo.startOffset + offset;
	feature.weight = 1;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
	LogFeatureInfo(featureInfo, feature, "%s %u", oss.str().c_str(), offset);
#endif
}

int CClickFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	switch (featureInfo.featureId)
	{
		case FEATURE_BAIDU_QUERY_URL:
			GetQueryUrl(featureInfo, oneCase, _baiduQueryUrlMap, features);
			break;
		case FEATURE_GOOGLE_QUERY_URL:
			GetQueryUrl(featureInfo, oneCase, _googleQueryUrlMap, features);
			break;
	}
	return 0;
}

