#include <fstream>
#include "BehaviorFeature.h"

namespace
{
    const char * MODULE_NAME = "Behavior Features" ;

    const uint32_t FEATURE_BEHAVIOR      = 400;

    SFeatureInfo supportedFeature[] = 
    {
        {"Behavior"     , FEATURE_BEHAVIOR      , 9000, 9100,  IMPLEMENT_BIDDING, BINARY_FEATURE, FEATURE_ENABLE}        
    };
};

CBehaviorFeature::CBehaviorFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CBehaviorFeature::CBehaviorFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CBehaviorFeature::Initialize(const string &modelPath)
{   
    if (!_isOnline)
    {
        CRegDataCenter::Instance()->_user2pvs.clear();
    }
    return 0;
}

void CBehaviorFeature::GetBehavior(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const
{
    feature.id = featureInfo.startOffset + oneCase.behavior;
    feature.weight = 1.0;

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "");
#endif
}


int CBehaviorFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{

    IdWeight feature;
    switch (featureInfo.featureId)
    {
    case FEATURE_BEHAVIOR:
        GetBehavior(featureInfo, oneCase, feature);
        break;
    }
    features.push_back(feature);

    return 0;
}

