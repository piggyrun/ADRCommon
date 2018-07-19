#include <fstream>
#include "LqyFeature.h"
#include "DataProcessor.h"

//#define FEATURE_DEBUG
namespace
{
    const char * MODULE_NAME = "Lqy Features" ;

    const uint32_t FEATURE_QUERY         = 700;
    const uint32_t FEATURE_ACCID_REL     = 701;
    const uint32_t FEATURE_ACCID_LOCALE  = 702;

    const uint32_t QUERY_BUCKET_SIZE  = 2000000;
    const uint32_t ACCID_LOCALE_BUCKET_SIZE = 400000;
    const uint32_t ACCID_REL_BUCKET_SIZE          = 100000;
    
    const char * QUERY_FILE  = "query";
    const char * ACCID_LOCALE_FILE = "accid_locale";
    const char * ACCID_REL_FILE = "acc_rel2ctr";

    SFeatureInfo supportedFeature[] = 
    {
        {"Query"      , FEATURE_QUERY        , 30000,  30199,  IMPLEMENT_RERANK,   BINARY_FEATURE, FEATURE_ENABLE},
        {"AccidLocale", FEATURE_ACCID_LOCALE , 30200,  30399,  IMPLEMENT_BIDDING,  BINARY_FEATURE, FEATURE_ENABLE},
        {"AccidRel"   , FEATURE_ACCID_REL    , 30400,  30599,  IMPLEMENT_RERANK,   BINARY_FEATURE, FEATURE_ENABLE}
    };
};

CLqyFeature::CLqyFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{

}

CLqyFeature::CLqyFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CLqyFeature::Initialize(const string &modelPath)
{  
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            uint32_t result = 0;            
            switch (_featureList[i].featureId)
            {
            case FEATURE_QUERY:
                result = LoadCtrBinaryMap(modelPath.c_str(), QUERY_FILE, _queryMap, QUERY_BUCKET_SIZE, true);
                break;
            case FEATURE_ACCID_REL:
                result = LoadCtrBinaryMap(modelPath.c_str(), ACCID_REL_FILE, _accidRelMap, ACCID_REL_BUCKET_SIZE, true);
                break;
            case FEATURE_ACCID_LOCALE:
                result = LoadCtrBinaryMap(modelPath.c_str(), ACCID_LOCALE_FILE, _accidLocaleMap, ACCID_LOCALE_BUCKET_SIZE, true);
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




void CLqyFeature::GetQuery(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    char tmp[256];

    sprintf(tmp, "%s", oneCase.queryAdInfo.query.c_str());

    md5_long_32 md5_output = getSign32(tmp, -1);
    uint32_t key = md5_output.data.intData[0];
     
    if (_queryMap.find(key, feature.id) == -1)
    {
        feature.id = 0; 
    }    

    feature.id = feature.id + featureInfo.startOffset;
    feature.weight = 1.0;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s", oneCase.queryAdInfo.query.c_str());
#endif

}

void CLqyFeature::GetAccidLocale(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    char tmp[256];

    sprintf(tmp, "%u\3%u", oneCase.queryAdInfo.accountid, oneCase.visitInfo.locale);

    md5_long_32 md5_output = getSign32(tmp, -1);
    uint32_t key = md5_output.data.intData[0];
     
    if (_accidLocaleMap.find(key, feature.id) == -1)
    {
        feature.id = 0; 
    }    

    feature.id = feature.id + featureInfo.startOffset;
    feature.weight = 1.0;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%u %u", oneCase.queryAdInfo.accountid, oneCase.visitInfo.locale);
#endif

}

void CLqyFeature::GetAccidRel(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    uint32_t rel;

    if (oneCase.rankInfo.relevance < 18)
    {
        rel = 0;
    }
    else if (oneCase.rankInfo.relevance > 25)
    {
        rel = 2;
    }
    else
    {
        rel = 1;
    }
    char tmp[256];

    sprintf(tmp, "%u\3%u", oneCase.queryAdInfo.accountid, rel);

    md5_long_32 md5_output = getSign32(tmp, -1);
    uint32_t key = md5_output.data.intData[0];
     
    if (_accidRelMap.find(key, feature.id) == -1)
    {
        feature.id = 0; 
    }    

    feature.id = feature.id + featureInfo.startOffset;
    feature.weight = 1.0;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%u %u %f", oneCase.queryAdInfo.accountid, rel, oneCase.rankInfo.relevance);
#endif

}

int CLqyFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{

    IdWeight feature;
    switch (featureInfo.featureId)
    {
    case FEATURE_QUERY:
        GetQuery(featureInfo, oneCase, feature);
        break;
    case FEATURE_ACCID_LOCALE:
        GetAccidLocale(featureInfo, oneCase, feature);
        break;
    case FEATURE_ACCID_REL:
        GetAccidRel(featureInfo, oneCase, feature);
        break;

    }

    features.push_back(feature);

    return 0;
}

