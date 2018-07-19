#ifndef LQY_FEATURE_H
#define LQY_FEATURE_H

#include "BaseFeature.h"

class CLqyFeature: public CBaseFeature
{
public:    

    CLqyFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CLqyFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CLqyFeature() {};       

    virtual int Initialize(const string &modelPath);

private:     

    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    void GetQuery(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    void GetAccidRel(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;

    void GetAccidLocale(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, IdWeight &feature) const;


private:    
    Int2IntHash _queryMap;

    Int2IntHash _accidRelMap;

    Int2IntHash _accidLocaleMap;

};

#endif
