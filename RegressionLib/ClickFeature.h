#ifndef CLICK_FEATURE_H
#define CLICK_FEATURE_H

#include "BaseFeature.h"

class CClickFeature: public CBaseFeature
{
public:    

    CClickFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CClickFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CClickFeature() {};       

    virtual int Initialize(const string &modelPath);

private:     

    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	// get specific feature tools
	void GetQueryUrl(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, const Int2IntHash &data_map, vector<IdWeight> &features) const;

private:    
    Int2IntHash _baiduQueryUrlMap;
    Int2IntHash _googleQueryUrlMap;
};

#endif
