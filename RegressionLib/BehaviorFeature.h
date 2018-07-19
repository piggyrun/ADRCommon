#ifndef BEHAVIOR_FEATURE_H
#define BEHAVIOR_FEATURE_H

#include "BaseFeature.h"

class CBehaviorFeature: public CBaseFeature
{
public:    

    CBehaviorFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CBehaviorFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CBehaviorFeature() {};

    virtual int Initialize(const string &modelPath);

private: 
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    void GetBehavior(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    void GetBehaviorOnline(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    void GetBehaviorOffline(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    
private:


};

#endif
