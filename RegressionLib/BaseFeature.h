#ifndef BASE_FEATURE_H
#define BASE_FEATURE_H

#include <vector>
#include <map>
#include <iostream>
#include "FeatureTable.h"
#include "RegressionCommon.h"
#include "RegDataCenter.h"
#include "../VectorHelper.h"

//#define FEATURE_DEBUG

class CBaseFeature
{
public:

    CBaseFeature(const char* moduleName, bool isOnline, SFeatureInfo *supportedFeature, size_t featureCount, uint32_t implementation = IMPLEMENT_ALL);

    CBaseFeature(const char* moduleName, bool isOnline, SFeatureInfo *supportedFeature, size_t featureCount, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CBaseFeature() {};    

    virtual int GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    void GetSupportedFeatures(vector<SFeatureInfo> &featureList) const;

    virtual int Initialize(const string &modelPath) = 0;

    string GetModuleName()
    {
        return _moduleName;
    }

protected:
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const = 0 ;    
   
    void LogFeatureInfo(const SFeatureInfo &featureInfo, IdWeight &newFeature, const char *information, ...) const;



private:
    void SetUseFeatures(const vector<uint32_t> &useFeatureList);

    void SetImplementationFeatures(uint32_t implementation);

    
protected:
    bool _isOnline;
    vector<SFeatureInfo> _featureList;
    
    map<uint32_t, uint32_t> _useFeatureMap;

    string _moduleName;

private:

};

#endif
