#ifndef TERM_WEIGHT_FEATURE_H
#define TERM_WEIGHT_FEATURE_H

#include "BaseFeature.h"

class CTermWeightFeature : public CBaseFeature
{
public:

	CTermWeightFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

	CTermWeightFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

	virtual ~CTermWeightFeature() {};

//	virtual int GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	virtual int Initialize(const string &modelPath);

private:

	virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	void GetCoreTermMatch(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	
};

#endif
