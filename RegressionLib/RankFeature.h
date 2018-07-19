#ifndef RANK_FEATURE_H
#define RANK_FEATURE_H

#include "BaseFeature.h"

class CRankFeature: public CBaseFeature
{
public:    

    CRankFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CRankFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CRankFeature() {};       

    virtual int GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    virtual int Initialize(const string &modelPath);

private:     
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    int LoadRankCtr(const char *modelPath, const char *fileName, vector<float> &rank2ctr, vector<uint32_t> &rank2id);

    int GetPos(float rank) const;

private:
    vector<vector<float> >     _rank2ctr;
    vector<vector<uint32_t> >  _rank2id;

};

#endif
