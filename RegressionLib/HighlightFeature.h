#ifndef HIGHLIGHT_FEATURE_H
#define HIGHLIGHT_FEATURE_H

#include "BaseFeature.h"

class CHighlightFeature: public CBaseFeature
{
public:    

    CHighlightFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CHighlightFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CHighlightFeature() {};

    virtual int Initialize(const string &modelPath);

private: 
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	void GetHighlight(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetHighlightQT(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetMatchCoverQT(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
    
private:

};

#endif
