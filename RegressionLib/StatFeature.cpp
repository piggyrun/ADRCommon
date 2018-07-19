#include <fstream>
#include "StatFeature.h"
#include "DataProcessor.h"

//#define FEATURE_DEBUG
namespace
{
    const char * MODULE_NAME = "Stat Features" ;

    const uint32_t FEATURE_QUERY_ACCID    = 300;
    const uint32_t FEATURE_ACCID_GROUND_M = 301;
    const uint32_t FEATURE_PID_M          = 302;
    const uint32_t FEATURE_QUERYLEN_M     = 303;
    const uint32_t FEATURE_ACCID_GROUND   = 311;
    const uint32_t FEATURE_PID            = 312;
    const uint32_t FEATURE_QUERYLEN       = 313;

	const uint32_t FEATURE_QUERY_SITE     = 320;
    const uint32_t FEATURE_ACCID_REL      = 321;

	const uint32_t FEATURE_SITE			  = 322;
    const uint32_t FEATURE_QUERY          = 323;
    const uint32_t FEATURE_SITE_REL       = 324;
    const uint32_t FEATURE_QUERY_URL      = 325;
    const uint32_t FEATURE_QTYPE_SITE     = 326;

    const uint32_t QUERY_ACCID_BUCKET_SIZE  = 20000000;
    const uint32_t ACCID_GROUND_BUCKET_SIZE = 100000;
    const uint32_t PID_BUCKET_SIZE          = 20000;
    const uint32_t QUERYLEN_BUCKET_SIZE     = 100;
    const uint32_t QUERY_BUCKET_SIZE  = 2000000;
    const uint32_t ACCID_REL_BUCKET_SIZE = 100000;

    const uint32_t SITE_REL_BUCKET_SIZE = 4000000;
    const uint32_t QUERY_SITE_BUCKET_SIZE  = 10000000;
    const uint32_t SITE_BUCKET_SIZE = 3000000;
	const uint32_t QUERY_URL_BUCKET_SIZE = 10000000;
	const uint32_t QTYPE_SITE_BUCKET_SIZE = 3000000;
    
    const char * QUERY_ACCID_FILE  = "query_accid";
    const char * ACCID_GROUND_FILE = "accid_gnd";
    const char * PID_FILE          = "pid";
    const char * QUERYLEN_FILE     = "querylen";
    const char * QUERY_FILE  	   = "query";
    const char * ACCID_REL_FILE    = "acc_rel2ctr";

    const char * SITE_REL_FILE     = "site_rel2ctr";
    const char * QUERY_SITE_FILE   = "query_site";
    const char * SITE_FILE     	   = "site";
    const char * QUERY_URL_FILE	   = "query_url";
    const char * QTYPE_SITE_FILE   = "qtype_site";

    const uint32_t  MAX_QUERY_LEN  = 32;
	const uint32_t FEATURE_SPAN_POSITION = 200; // 结合位置后，每个位置区间的大小
	const uint32_t MAX_STAT_POSTION = 5; // 统计的位置

	const int QUERY_TYPE_INDEX[] = {13, 18, 19, 29, 30, 31, 32, 34, 35, 37, 38, 39, 40};
	const size_t QTYPE_NUM = sizeof(QUERY_TYPE_INDEX)/sizeof(int);

    SFeatureInfo supportedFeature[] = 
    {
        {"AccidGnd_M"      , FEATURE_ACCID_GROUND_M , 5000,   5000,   IMPLEMENT_RERANK,  MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"QueryLen_M"      , FEATURE_QUERYLEN_M     , 5002,   5002,   IMPLEMENT_RERANK,  MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"Pid_M"           , FEATURE_PID_M          , 5001,   5001,   IMPLEMENT_BIDDING, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"QueryAccid"      , FEATURE_QUERY_ACCID    , 5100,   5299,   IMPLEMENT_RERANK,  BINARY_FEATURE,      FEATURE_DISABLE},
        {"AccidGnd"        , FEATURE_ACCID_GROUND   , 5300,   5499,   IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_DISABLE},
        {"QueryLen"        , FEATURE_QUERYLEN       , 5500,   5699,   IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_DISABLE},
        {"Pid"             , FEATURE_PID            , 5700,   5899,   IMPLEMENT_BIDDING, BINARY_FEATURE, 	  FEATURE_DISABLE},
        {"Query"           , FEATURE_QUERY          , 30000,  30199,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_DISABLE},
        {"AccidRel"   	   , FEATURE_ACCID_REL      , 30200,  30399,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_DISABLE},

        {"SiteRel"   	   , FEATURE_SITE_REL       , 31000,  31999,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE},
        {"QuerySite"   	   , FEATURE_QUERY_SITE     , 32000,  32999,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE},
        {"Site"		   	   , FEATURE_SITE           , 33000,  33999,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE},
        {"QueryUrl"	   	   , FEATURE_QUERY_URL      , 34000,  34999,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE},
        {"QtypeSite"	   , FEATURE_QTYPE_SITE     , 35000,  35999,  IMPLEMENT_RERANK,  BINARY_FEATURE, 	  FEATURE_ENABLE}
    };
};

CStatFeature::CStatFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CStatFeature::CStatFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CStatFeature::Initialize(const string &modelPath)
{  
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            uint32_t result = 0;            
            switch (_featureList[i].featureId)
            {
            case FEATURE_ACCID_GROUND_M:
                result = LoadCtrMap(modelPath.c_str(), ACCID_GROUND_FILE, _accidGroundCtrMap, ACCID_GROUND_BUCKET_SIZE, false);
                break;
			case FEATURE_QUERYLEN_M:
                result = LoadCtrMap(modelPath.c_str(), QUERYLEN_FILE, _querylenCtrMap, QUERYLEN_BUCKET_SIZE, false);
                break;
            case FEATURE_PID_M:
                result = LoadCtrMap(modelPath.c_str(), PID_FILE, _pidCtrMap, PID_BUCKET_SIZE, true);
                break;
            case FEATURE_QUERY_ACCID:
                result = LoadCtrBinaryMap(modelPath.c_str(), QUERY_ACCID_FILE, _queryAccidMap, QUERY_ACCID_BUCKET_SIZE, true);
                break;
            case FEATURE_ACCID_GROUND:
                result = LoadCtrBinaryMap(modelPath.c_str(), ACCID_GROUND_FILE, _accidGroundMap, ACCID_GROUND_BUCKET_SIZE, false);
                break;
            case FEATURE_PID:
                result = LoadCtrPidMap(modelPath.c_str(), PID_FILE, _pidMap, PID_BUCKET_SIZE);
                break;
            case FEATURE_QUERY:
                result = LoadCtrBinaryMap(modelPath.c_str(), QUERY_FILE, _queryMap, QUERY_BUCKET_SIZE, true);
                break;
            case FEATURE_ACCID_REL:
                result = LoadCtrBinaryMap(modelPath.c_str(), ACCID_REL_FILE, _accidRelMap, ACCID_REL_BUCKET_SIZE, true);
                break;

            case FEATURE_SITE_REL:
                result = LoadCtrBinaryMap(modelPath.c_str(), SITE_REL_FILE, _siteRelMap, SITE_REL_BUCKET_SIZE, true);
                break;
            case FEATURE_QUERY_SITE:
                result = LoadCtrBinaryMap(modelPath.c_str(), QUERY_SITE_FILE, _querySiteMap, QUERY_SITE_BUCKET_SIZE, true);
                break;
            case FEATURE_SITE:
                result = LoadCtrBinaryMap(modelPath.c_str(), SITE_FILE, _siteMap, SITE_BUCKET_SIZE, true);
                break;
            case FEATURE_QUERY_URL:
                result = LoadCtrBinaryMap(modelPath.c_str(), QUERY_URL_FILE, _queryUrlMap, QUERY_URL_BUCKET_SIZE, true);
                break;
            case FEATURE_QTYPE_SITE:
                result = LoadCtrBinaryMap(modelPath.c_str(), QTYPE_SITE_FILE, _qtypeSiteMap, QTYPE_SITE_BUCKET_SIZE, true);
                break;
            }
            if (result != 0)
            {
                throw;
            }
        }
    }
    return 0;
}

void CStatFeature::GetQueryAccid(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    char tmp[256];

    sprintf(tmp, "%s\3%u", oneCase.queryAdInfo.query.c_str(), oneCase.queryAdInfo.accountid);

    md5_long_32 md5_output = getSign32(tmp, -1);
    uint32_t key = md5_output.data.intData[0];
     
    if (_queryAccidMap.find(key, feature.id) == -1)
    {
        feature.id = 0; 
    }    

    feature.id = feature.id + featureInfo.startOffset;
    feature.weight = 1.0;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s %u", oneCase.queryAdInfo.query.c_str(), oneCase.queryAdInfo.accountid);
#endif

}

void CStatFeature::GetAccidGroundCtr(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    uint32_t key = oneCase.queryAdInfo.accountid;
     
    feature.id = featureInfo.startOffset;
    if (_accidGroundCtrMap.find(key, feature.weight) == -1)
    {
        feature.weight = CRegDataCenter::Instance()->GetAvgCtr(); 
    } 
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%u", oneCase.queryAdInfo.accountid);
#endif

}

void CStatFeature::GetPidCtr(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    feature.id = featureInfo.startOffset;
    if (oneCase.visitInfo.channel != 0)
    {
        feature.weight = CRegDataCenter::Instance()->GetAvgCtr(); 
		features.push_back(feature);
#ifdef FEATURE_DEBUG
        LogFeatureInfo(featureInfo, feature, "Other Channel, %s", oneCase.visitInfo.pid.c_str());
#endif
        return;
    }

    md5_long_32 md5_output = getSign32(oneCase.visitInfo.pid.c_str(), -1);
    uint32_t key = md5_output.data.intData[0];

    if (_pidCtrMap.find(key, feature.weight) == -1)
    {
        feature.weight = CRegDataCenter::Instance()->GetAvgCtr(); 
    } 
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s", oneCase.visitInfo.pid.c_str());
#endif


}

void CStatFeature::GetQuerylenCtr(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    uint32_t key = oneCase.queryAdInfo.query.size();
     
    feature.id = featureInfo.startOffset;
    if (_querylenCtrMap.find(key, feature.weight) == -1)
    {
        feature.weight = CRegDataCenter::Instance()->GetAvgCtr(); 
    } 
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s", oneCase.queryAdInfo.query.c_str());
#endif

}

void CStatFeature::GetAccidGround(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    uint32_t key = oneCase.queryAdInfo.accountid;
    
    uint32_t offset = 0;

    feature.id = featureInfo.startOffset;
    if (_accidGroundMap.find(key, offset) == -1)
    {
        offset = 0;
    } 

    feature.id = featureInfo.startOffset + offset;
    feature.weight = 1;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%u", oneCase.queryAdInfo.accountid);
#endif

}

void CStatFeature::GetPid(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    uint32_t offset = 0;

    string pid = oneCase.visitInfo.pid;
    if (pid == "sogou-clse-d3e241cdebeed984")
    {
        pid = "sogou-site-a5912d5771cbddba";
    }
    md5_long_32 md5_output = getSign32(oneCase.visitInfo.pid.c_str(), -1);
    uint32_t key = md5_output.data.intData[0];

    if (_pidMap.find(key, offset) == -1)
    {
        offset = 0;
    } 

    feature.id = featureInfo.startOffset + offset;
    feature.weight = 1;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s", oneCase.visitInfo.pid.c_str());
#endif
}

void CStatFeature::GetQuerylen(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    uint32_t len = oneCase.queryAdInfo.query.size();
    if (len > MAX_QUERY_LEN)
    {
        len = MAX_QUERY_LEN;
    }
    feature.id = featureInfo.startOffset + len;
    feature.weight = 1;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s", oneCase.queryAdInfo.query.c_str());
#endif

}

void CStatFeature::GetQuery(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    IdWeight feature;
    char tmp[256];

    sprintf(tmp, "%s", oneCase.queryAdInfo.query.c_str());

    md5_long_32 md5_output = getSign32(tmp, -1);
    uint32_t key = md5_output.data.intData[0];
     
    if (_queryMap.find(key, feature.id) == -1)
    {
        feature.id = 0; 
    }    

    feature.id = feature.id + featureInfo.startOffset;
    feature.weight = 1.0;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%s", oneCase.queryAdInfo.query.c_str());
#endif

}

void CStatFeature::GetAccidRel(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    uint32_t rel;
    IdWeight feature;
    if (oneCase.rankInfo.relevance < 18)
    {
        rel = 0;
    }
    else if (oneCase.rankInfo.relevance > 25)
    {
        rel = 2;
    }
    else
    {
        rel = 1;
    }
    char tmp[256];

    sprintf(tmp, "%u\3%u", oneCase.queryAdInfo.accountid, rel);

    md5_long_32 md5_output = getSign32(tmp, -1);
    uint32_t key = md5_output.data.intData[0];
     
    if (_accidRelMap.find(key, feature.id) == -1)
    {
        feature.id = 0; 
    }    

    feature.id = feature.id + featureInfo.startOffset;
    feature.weight = 1.0;
    features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "%u %u %f", oneCase.queryAdInfo.accountid, rel, oneCase.rankInfo.relevance);
#endif

}

void CStatFeature::GetSiteRel(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    uint32_t rel;
    if (oneCase.webRankInfo.web_rank < 15) // ctr < 40%
    {
        rel = 0;
    }
    else if (oneCase.webRankInfo.web_rank > 25) // ctr > 60%
    {
        rel = 2;
    }
    else // avg ctr 50%
    {
        rel = 1;
    }

	bool is_find = false; // 只取最高位，其它均用默认
	for (size_t i=0; i<MAX_STAT_POSTION; i++)
	{
		IdWeight feature;
		ostringstream oss; 
		oss << oneCase.queryDocInfo.site << '\3' << rel << '\3' << (i+1);
		md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
		uint32_t key = md5_output.data.intData[0];
		uint32_t offset = 0;
		if (!is_find)
		{
			if (_siteRelMap.find(key, offset) == -1)
				offset = 0; 
			else
				is_find = true;
		}
		feature.id = featureInfo.startOffset + FEATURE_SPAN_POSITION*i + offset;
		feature.weight = 1.0;
		features.push_back(feature);

#ifdef FEATURE_DEBUG
		LogFeatureInfo(featureInfo, feature, "%s %f %u", oss.str().c_str(), oneCase.webRankInfo.web_rank, offset);
#endif
	}

}

void CStatFeature::GetQuerySite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	bool is_find = false; // 只取最高位，其它均用默认
	for (size_t i=0; i<MAX_STAT_POSTION; i++)
	{
		IdWeight feature;
		ostringstream oss; 
		oss << oneCase.queryDocInfo.query << '\3' << oneCase.queryDocInfo.site << '\3' << (i+1);
		md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
		uint32_t key = md5_output.data.intData[0];
		uint32_t offset = 0;
		if (!is_find)
		{
			if (_querySiteMap.find(key, offset) == -1)
				offset = 0; 
			else
				is_find = true;
		}
		feature.id = featureInfo.startOffset + FEATURE_SPAN_POSITION*i + offset;
		feature.weight = 1.0;
		features.push_back(feature);

#ifdef FEATURE_DEBUG
		LogFeatureInfo(featureInfo, feature, "%s %u", oss.str().c_str(), offset);
#endif
	}
}

void CStatFeature::GetSite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	bool is_find = false; // 只取最高位，其它均用默认
	for (size_t i=0; i<MAX_STAT_POSTION; i++)
	{
		IdWeight feature;
		ostringstream oss; 
		oss << oneCase.queryDocInfo.site << '\3' << (i+1);
		md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
		uint32_t key = md5_output.data.intData[0];
		uint32_t offset = 0;
		if (!is_find)
		{
			if (_siteMap.find(key, offset) == -1)
				offset = 0;
			else
				is_find = true;
		}
		feature.id = featureInfo.startOffset + FEATURE_SPAN_POSITION*i + offset;
		feature.weight = 1;
		features.push_back(feature);

#ifdef FEATURE_DEBUG
		LogFeatureInfo(featureInfo, feature, "%s %u", oss.str().c_str(), offset);
#endif
	}
}

void CStatFeature::GetQueryUrl(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	bool is_find = false; // 只取最高位，其它均用默认
	for (size_t i=0; i<MAX_STAT_POSTION; i++)
	{
		IdWeight feature;
		ostringstream oss; 
		oss << oneCase.queryDocInfo.query << '\3' << oneCase.queryDocInfo.url << '\3' << (i+1);
		md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
		uint32_t key = md5_output.data.intData[0];
		uint32_t offset = 0;
		if (!is_find)
		{
			if (_queryUrlMap.find(key, offset) == -1)
				offset = 0;
			else
				is_find = true;
		}
		feature.id = featureInfo.startOffset + FEATURE_SPAN_POSITION*i + offset;
		feature.weight = 1;
		features.push_back(feature);

#ifdef FEATURE_DEBUG
		LogFeatureInfo(featureInfo, feature, "%s %u", oss.str().c_str(), offset);
#endif
	}
}

void CStatFeature::GetQtypeSite(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	for (size_t j=0; j<QTYPE_NUM; j++)
	{
		if (((oneCase.queryDocInfo.flag>>QUERY_TYPE_INDEX[j])&1) == 1) // query has a type
		{
			bool is_find = false; // 只取最高位，其它均用默认
			for (size_t i=0; i<MAX_STAT_POSTION; i++)
			{
				IdWeight feature;
				ostringstream oss;
				oss << QUERY_TYPE_INDEX[j] << '\3' << oneCase.queryDocInfo.site << '\3' << (i+1);
				md5_long_32 md5_output = getSign32(oss.str().c_str(), -1);
				uint32_t key = md5_output.data.intData[0];
				uint32_t offset = 0;
				if (!is_find)
				{
					if (_qtypeSiteMap.find(key, offset) == -1)
						offset = 0;
					else
						is_find = true;
				}
				feature.id = featureInfo.startOffset + FEATURE_SPAN_POSITION*i + offset;
				feature.weight = 1;
				features.push_back(feature);
#ifdef FEATURE_DEBUG
				LogFeatureInfo(featureInfo, feature, "%s %u", oss.str().c_str(), offset);
#endif
			}
			break; // 只取最先找到的一个类别
		}
	}
}

int CStatFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	switch (featureInfo.featureId)
	{
		case FEATURE_ACCID_GROUND_M:
			GetAccidGroundCtr(featureInfo, oneCase, features);
			break;
		case FEATURE_QUERYLEN_M:
			GetQuerylenCtr(featureInfo, oneCase, features);
			break;
		case FEATURE_PID_M:
			GetPidCtr(featureInfo, oneCase, features);
			break;
		case FEATURE_QUERY_ACCID:
			GetQueryAccid(featureInfo, oneCase, features);
			break;
		case FEATURE_ACCID_GROUND:
			GetAccidGround(featureInfo, oneCase, features);
			break;
		case FEATURE_QUERYLEN:
			GetQuerylen(featureInfo, oneCase, features);
			break;
		case FEATURE_PID:
			GetPid(featureInfo, oneCase, features);
			break;
		case FEATURE_QUERY:
			GetQuery(featureInfo, oneCase, features);
			break;
		case FEATURE_ACCID_REL:
			GetAccidRel(featureInfo, oneCase, features);
			break;

		case FEATURE_SITE_REL:
			GetSiteRel(featureInfo, oneCase, features);
			break;
		case FEATURE_QUERY_SITE:
			GetQuerySite(featureInfo, oneCase, features);
			break;
		case FEATURE_SITE:
			GetSite(featureInfo, oneCase, features);
			break;
		case FEATURE_QUERY_URL:
			GetQueryUrl(featureInfo, oneCase, features);
			break;
		case FEATURE_QTYPE_SITE:
			GetQtypeSite(featureInfo, oneCase, features);
			break;
	}
	return 0;
}
