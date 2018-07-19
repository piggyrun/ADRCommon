#ifndef SITE_FEATURE_H
#define SITE_FEATURE_H

#include "BaseFeature.h"

class CSiteFeature: public CBaseFeature
{
public:    

    CSiteFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CSiteFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CSiteFeature() {};

    virtual int Initialize(const string &modelPath);

private: 
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	void GetBigSite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetSiteRank(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

private:
    Int2IntHash _bigSiteMap;
	Long2IntHash _siteRankMap;

};

#endif
