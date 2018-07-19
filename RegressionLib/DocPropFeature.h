#ifndef DOC_PROP_FEATURE_H
#define DOC_PROP_FEATURE_H

#include "BaseFeature.h"

class CDocPropFeature: public CBaseFeature
{
public:    

    CDocPropFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CDocPropFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CDocPropFeature() {};

    virtual int Initialize(const string &modelPath);

private: 
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	void GetDocPos(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetDocType(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetDocTypePos(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

private:

};

#endif
