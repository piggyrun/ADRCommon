#ifndef BIDDING_FEATURE_H
#define BIDDING_FEATURE_H

#include "BaseFeature.h"

class CBiddingFeature: public CBaseFeature
{
public:    

    CBiddingFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CBiddingFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CBiddingFeature() {};       

    virtual int Initialize(const string &modelPath);

private: 
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    void GetAdCount(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    void GetCateLocale(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    void GetCateHour(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    
private:
    uint32_t _cateCount;


};

#endif
