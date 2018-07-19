#include <fstream>
#include "DocPropFeature.h"

namespace
{
    const char * MODULE_NAME = "Doc property Features" ;

	const uint32_t MAX_DOC_POS = 10;
	const uint32_t MAX_DOC_TYPE_NUM = 64;

    const uint32_t FEATURE_DOC_TYPE      = 400;
    const uint32_t FEATURE_DOC_POS       = 401;
    const uint32_t FEATURE_DOC_TYPE_POS  = 402;

    SFeatureInfo supportedFeature[] = 
    {
        {"DocPos"      , FEATURE_DOC_POS       , 9000, 9099,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},       
        {"DocType"     , FEATURE_DOC_TYPE      , 9100, 9299,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"DocTypePos"  , FEATURE_DOC_TYPE_POS  , 9300, 10999, IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE}
    };
};

CDocPropFeature::CDocPropFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CDocPropFeature::CDocPropFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CDocPropFeature::Initialize(const string &modelPath)
{   
    return 0;
}

void CDocPropFeature::GetDocPos(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	IdWeight feature;
    feature.id = featureInfo.startOffset + oneCase.position;
    feature.weight = 1.0;
	features.push_back(feature);

#ifdef FEATURE_DEBUG
    LogFeatureInfo(featureInfo, feature, "pos: %u", oneCase.position);
#endif
}

void CDocPropFeature::GetDocType(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	for (size_t i=0; i<MAX_DOC_TYPE_NUM; i++)
	{
		if (((oneCase.queryDocInfo.flag>>i)&1) == 1) // this type exists
		{
			IdWeight feature;
			feature.id = featureInfo.startOffset + i;
			feature.weight = 1.0;
			features.push_back(feature);

#ifdef FEATURE_DEBUG
			LogFeatureInfo(featureInfo, feature, "type: %u", i);
#endif
		}
	}
}

void CDocPropFeature::GetDocTypePos(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
	for (size_t i=0; i<MAX_DOC_TYPE_NUM; i++)
	{
		if (((oneCase.queryDocInfo.flag>>i)&1) == 1) // this type exists
		{
			IdWeight feature;
			feature.id = featureInfo.startOffset + (oneCase.position - 1) + MAX_DOC_POS*i;
			feature.weight = 1.0;
			features.push_back(feature);

#ifdef FEATURE_DEBUG
			LogFeatureInfo(featureInfo, feature, "type: %u, pos: %u", i, oneCase.position);
#endif
		}
	}
}

int CDocPropFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    switch (featureInfo.featureId)
    {
    case FEATURE_DOC_POS:
        GetDocPos(featureInfo, oneCase, features);
        break;
	case FEATURE_DOC_TYPE:
		GetDocType(featureInfo, oneCase, features);
		break;
	case FEATURE_DOC_TYPE_POS:
		GetDocTypePos(featureInfo, oneCase, features);
		break;
    }

    return 0;
}

