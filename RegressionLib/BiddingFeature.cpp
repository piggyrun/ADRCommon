#include <fstream>
#include "BiddingFeature.h"

namespace
{
    const char * MODULE_NAME = "Bidding Features" ;

    const uint32_t FEATURE_ADCOUNT       = 200;
    const uint32_t FEATURE_CATE_LOCALE   = 201;
    const uint32_t FEATURE_CATE_HOUR     = 202;

    const uint32_t MAX_ADCOUNT  = 3;

    SFeatureInfo supportedFeature[] = 
    {
        {"Adcount"      , FEATURE_ADCOUNT      , 10000, 10099,  IMPLEMENT_BIDDING, BINARY_FEATURE, FEATURE_ENABLE},
        {"CateHour"     , FEATURE_CATE_HOUR    , 10100, 14999,  IMPLEMENT_BIDDING, BINARY_FEATURE, FEATURE_ENABLE},
        {"CateLocale"   , FEATURE_CATE_LOCALE  , 15000, 19999,  IMPLEMENT_BIDDING, BINARY_FEATURE, FEATURE_ENABLE}
        
    };
};

CBiddingFeature::CBiddingFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CBiddingFeature::CBiddingFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CBiddingFeature::Initialize(const string &modelPath)
{
    _cateCount = CRegDataCenter::Instance()->GetCateCount();

    return 0;
}


void CBiddingFeature::GetAdCount(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    uint32_t adcount = oneCase.adcount;
    if (adcount > MAX_ADCOUNT)
    {
        adcount = MAX_ADCOUNT;
    }
    feature.id = featureInfo.startOffset + adcount;
    feature.weight = 1;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%d", oneCase.adcount);
#endif

}

void CBiddingFeature::GetCateHour(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    uint32_t id = oneCase.queryAdInfo.categoryIndex * HOUR_COUNT + oneCase.visitInfo.pvhour;
    feature.id = featureInfo.startOffset + id;
    feature.weight = 1;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%d, %d", oneCase.queryAdInfo.categoryIndex, oneCase.visitInfo.pvhour);
#endif

}

void CBiddingFeature::GetCateLocale(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    uint32_t id = oneCase.queryAdInfo.categoryIndex * LOCALE_COUNT + oneCase.visitInfo.locale;
    feature.id = featureInfo.startOffset + id;
    feature.weight = 1;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%d, %d", oneCase.queryAdInfo.categoryIndex, oneCase.visitInfo.locale);
#endif
}


int CBiddingFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    switch (featureInfo.featureId)
    {
    case FEATURE_ADCOUNT:
        GetAdCount(featureInfo, oneCase, feature);
        break;
    case FEATURE_CATE_LOCALE:
        GetCateLocale(featureInfo, oneCase, feature);
        break;
    case FEATURE_CATE_HOUR:
        GetCateHour(featureInfo, oneCase, feature);
        break;
    }
    features.push_back(feature);

    return 0;
}

