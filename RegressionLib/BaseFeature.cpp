#include "BaseFeature.h"

CBaseFeature::CBaseFeature(const char* moduleName, bool isOnline, SFeatureInfo *supportedFeature, size_t featureCount, uint32_t implementation)
    :_isOnline(isOnline)
     
{    
    _moduleName = moduleName;    

    for (uint32_t i = 0; i < featureCount; i++)
    {
        _featureList.push_back(supportedFeature[i]);
        _useFeatureMap[supportedFeature[i].featureId] = supportedFeature[i].isUse;
    }

    SetImplementationFeatures(implementation);

}

CBaseFeature::CBaseFeature(const char* moduleName, bool isOnline, SFeatureInfo *supportedFeature, size_t featureCount, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :_isOnline(isOnline)
{   
    _moduleName = moduleName;

    for (uint32_t i = 0; i < featureCount; i++)
    {
        _featureList.push_back(supportedFeature[i]);
    }
    SetUseFeatures(useFeatureList);
    SetImplementationFeatures(implementation);
}

int CBaseFeature::GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            GetFeature(_featureList[i], oneCase, features);
        }
    }

    return 0;
}


void CBaseFeature::GetSupportedFeatures(vector<SFeatureInfo> &featureList) const
{
    featureList = _featureList;
}

void CBaseFeature::SetUseFeatures(const vector<uint32_t> &useFeatureList)
{
    _useFeatureMap.clear();
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        uint32_t enable = FEATURE_DISABLE;
        for (size_t j = 0; j < useFeatureList.size(); j++)
        {
            if (_featureList[i].featureId == useFeatureList[j])
            {
                enable = FEATURE_ENABLE;
                break;
            }
        }
        _featureList[i].isUse = enable;
        _useFeatureMap[_featureList[i].featureId] = enable;
    }
}

void CBaseFeature::SetImplementationFeatures(uint32_t implementation)
{
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (implementation != IMPLEMENT_ALL && implementation != _featureList[i].implementation)
        {
            _featureList[i].isUse = FEATURE_DISABLE;
            _useFeatureMap[_featureList[i].featureId] = FEATURE_DISABLE;
        }
    }
}

void CBaseFeature::LogFeatureInfo(const SFeatureInfo &featureInfo, IdWeight &feature, const char *information, ...) const
{
    va_list args;
    va_start(args, information);    
    char buffer[256];
    vsnprintf(buffer, 256, information, args);        
    va_end(args);    

    CRegDataCenter::Instance()->LoggingNormal("Feature Name : %s\tFeature Id : %d\tFeature Weight : %f\tInformation : %s", featureInfo.featureName.c_str(), feature.id, feature.weight, buffer);

}
