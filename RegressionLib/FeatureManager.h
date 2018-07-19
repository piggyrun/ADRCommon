#ifndef FEATURE_MANAGER_H
#define FEATURE_MANAGER_H

#include "RegressionCommon.h"
#include "RegDataCenter.h"
#include "BaseFeature.h"

class CFeatureManager
{
public:
    CFeatureManager(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CFeatureManager(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    CFeatureManager(bool isOnline, const string &featureFile, uint32_t implementation = IMPLEMENT_ALL);

    int Initialize(const string &modelPath);

    void OutputFeatureList(const string filePath, bool isAll) const;

    uint32_t GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const;
    
    ~CFeatureManager();
private:
    void GenerateFeatureInfo();
    
private:
    vector<CBaseFeature *>  _featureClasses;
    
    vector<SFeatureInfo> _featureList;
};

#endif
