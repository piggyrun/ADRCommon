#ifndef STAT_FEATURE_H
#define STAT_FEATURE_H

#include "BaseFeature.h"

class CStatFeature: public CBaseFeature
{
public:    

    CStatFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CStatFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CStatFeature() {};       

    virtual int Initialize(const string &modelPath);

private:     

    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	// get specific feature tools
    void GetQueryAccid(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
    void GetAccidGroundCtr(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
    void GetPidCtr(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetQuerylenCtr(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
    void GetAccidGround(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
    void GetPid(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetQuerylen(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetQuery(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetAccidRel(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	void GetSiteRel(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetQuerySite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetSite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetQueryUrl(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;
	void GetQtypeSite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

private:    
    Int2IntHash _queryAccidMap;

    Int2IntHash _pidMap;

    Int2IntHash _accidGroundMap;

    Int2IntHash _querylenMap;


    Int2FloatHash _accidGroundCtrMap;

    Int2FloatHash _pidCtrMap;

    Int2FloatHash _querylenCtrMap;

	Int2IntHash _querySiteMap;
	Int2IntHash _siteMap;
    Int2IntHash _queryMap;
    Int2IntHash _accidRelMap;
    Int2IntHash _siteRelMap;
    Int2IntHash _queryUrlMap;
    Int2IntHash _qtypeSiteMap;
};

#endif
