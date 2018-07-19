#include <fstream>
#include "RankFeature.h"

namespace
{
    const char * MODULE_NAME = "Rank Features" ;

    const uint32_t MAX_RANK_NUM     = 20; // max rank num in each rank section
    const uint32_t AD_RANK_NUM      = 10;
	const uint32_t WEB_RANK_NUM     = 10;
    const uint32_t RANK_NUM         = AD_RANK_NUM + WEB_RANK_NUM;

	// ad ranks
    const uint32_t FEATURE_ABSOLUTE_RANK_M       = 100;
    const uint32_t FEATURE_TITLE_RANK_M          = 101;
    const uint32_t FEATURE_DESC_RANK_M           = 102;
    const uint32_t FEATURE_TITLE_HL_RANK_M       = 103;
    const uint32_t FEATURE_DESC_HL_RANK_M        = 104;
    const uint32_t FEATURE_MATCH_COVER_RANK_M    = 105;
    const uint32_t FEATURE_ONLY_MATCH_RANK_M     = 106;
    const uint32_t FEATURE_ONLY_NONMATCH_RANK_M  = 107;
    const uint32_t FEATURE_AD_EXT_RANK_M         = 108;
    const uint32_t FEATURE_CONCURRENCE_RANK_M    = 109;

    const uint32_t FEATURE_ABSOLUTE_RANK_B       = 120;
    const uint32_t FEATURE_TITLE_RANK_B          = 121;
    const uint32_t FEATURE_DESC_RANK_B           = 122;
    const uint32_t FEATURE_TITLE_HL_RANK_B       = 123;
    const uint32_t FEATURE_DESC_HL_RANK_B        = 124;
    const uint32_t FEATURE_MATCH_COVER_RANK_B    = 125;
    const uint32_t FEATURE_ONLY_MATCH_RANK_B     = 126;
    const uint32_t FEATURE_ONLY_NONMATCH_RANK_B  = 127;
    const uint32_t FEATURE_AD_EXT_RANK_B         = 128;
    const uint32_t FEATURE_CONCURRENCE_RANK_B    = 129;

	// web ranks: note 140~159 were used for multinomial rank features (not implemented)
	const uint32_t FEATURE_WEB_FIXED_RANK_B = 160;
	const uint32_t FEATURE_WEB_CONTENT_RANK_B = 161;
	const uint32_t FEATURE_WEB_KEY_BM25_RANK_B = 162;
	const uint32_t FEATURE_WEB_BM25_RANK_B = 163;
	const uint32_t FEATURE_WEB_MATCH_RANK_B = 164;
	const uint32_t FEATURE_WEB_PAGE_RANK_B = 165;
	const uint32_t FEATURE_WEB_USER_RANK_B = 166;
	const uint32_t FEATURE_WEB_TIME_RANK_B = 167;
	const uint32_t FEATURE_WEB_RANK_B = 168;
	const uint32_t FEATURE_WEB_FINAL_RANK_B = 169;

    const uint32_t RANK_FEATURE_MULTINOMIAL = 0;
    const uint32_t RANK_FEATURE_BINARY      = 1;

    const char * rankFile[RANK_NUM] = 
    {
		// ad
        "absolute",
        "title",
        "desc",
        "title_highlight",
        "desc_highlight",
        "match_cover",
        "only_match",
        "only_nonmatch",
        "ad_ext",
        "concur",
		// web
		"fixed_rank",
		"content_rank",
		"key_bm25_rank",
		"bm25_rank",
		"match_rank",
		"page_rank",
		"user_rank",
		"time_rank",
		"rank",
		"final_rank"
    };

    const uint32_t RANK_RANGE = 1002;

    SFeatureInfo supportedFeature[] = 
    {
        {"AbsolutionRank_M"   , FEATURE_ABSOLUTE_RANK_M      , 1,  1,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"TitleRank_M"        , FEATURE_TITLE_RANK_M         , 2,  2,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"DescRank_M"         , FEATURE_DESC_RANK_M          , 3,  3,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"TitleHLRank_M"      , FEATURE_TITLE_HL_RANK_M      , 4,  4,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"DescHLRank_M"       , FEATURE_DESC_HL_RANK_M       , 5,  5,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"MatchCoverRank_M"   , FEATURE_MATCH_COVER_RANK_M   , 6,  6,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"OnlyMatchRank_M"    , FEATURE_ONLY_MATCH_RANK_M    , 7,  7,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"OnlyNonMatchRank_M" , FEATURE_ONLY_NONMATCH_RANK_M , 8,  8,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"AdExtRank_M"        , FEATURE_AD_EXT_RANK_M        , 9,  9,  IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},
        {"ConcurrenceRank_M"  , FEATURE_CONCURRENCE_RANK_M   , 10, 10, IMPLEMENT_RANKDOC, MULTINOMIAL_FEATURE, FEATURE_DISABLE},

        {"AbsolutionRank_B"   , FEATURE_ABSOLUTE_RANK_B      , 100,  149,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"TitleRank_B"        , FEATURE_TITLE_RANK_B         , 150,  199,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"DescRank_B"         , FEATURE_DESC_RANK_B          , 200,  249,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"TitleHLRank_B"      , FEATURE_TITLE_HL_RANK_B      , 250,  299,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"DescHLRank_B"       , FEATURE_DESC_HL_RANK_B       , 300,  349,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"MatchCoverRank_B"   , FEATURE_MATCH_COVER_RANK_B   , 350,  399,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"OnlyMatchRank_B"    , FEATURE_ONLY_MATCH_RANK_B    , 400,  449,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"OnlyNonMatchRank_B" , FEATURE_ONLY_NONMATCH_RANK_B , 450,  499,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"AdExtRank_B"        , FEATURE_AD_EXT_RANK_B        , 500,  549,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},
        {"ConcurrenceRank_B"  , FEATURE_CONCURRENCE_RANK_B   , 550,  599,  IMPLEMENT_RANKDOC, BINARY_FEATURE, FEATURE_DISABLE},

		// web rank: sharing the same coeficient section with ad ranks
		// NOTE: each feature has 100 spaces...
        {"WebFixRank_B"       , FEATURE_WEB_FIXED_RANK_B      , 100,  199,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebContentRank_B"   , FEATURE_WEB_CONTENT_RANK_B    , 200,  299,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebKBRank_B"        , FEATURE_WEB_KEY_BM25_RANK_B   , 300,  399,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebBRank_B"         , FEATURE_WEB_BM25_RANK_B       , 400,  499,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebMatchRank_B"     , FEATURE_WEB_MATCH_RANK_B      , 500,  599,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebPageRank_B"      , FEATURE_WEB_PAGE_RANK_B       , 600,  699,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebUserRank_B"      , FEATURE_WEB_USER_RANK_B       , 700,  799,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebTimeRank_B"      , FEATURE_WEB_TIME_RANK_B       , 800,  899,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebRank_B" 		  , FEATURE_WEB_RANK_B			  , 900,  999,  IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE},
        {"WebFinalRank_B"     , FEATURE_WEB_FINAL_RANK_B      , 1000, 1099, IMPLEMENT_RERANK, BINARY_FEATURE, FEATURE_ENABLE}
    };

};

CRankFeature::CRankFeature(bool isOnline, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), implementation)
{
}

CRankFeature::CRankFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
    :CBaseFeature(MODULE_NAME, isOnline, supportedFeature, sizeof(supportedFeature) / sizeof(SFeatureInfo), useFeatureList, implementation)
{
}

int CRankFeature::Initialize(const string &modelPath)
{
    _rank2ctr.resize(RANK_NUM);
    _rank2id.resize(RANK_NUM);

    vector<uint32_t> isEnable(RANK_NUM);
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            uint32_t rankType = _featureList[i].featureId % MAX_RANK_NUM;
            uint32_t featureType = ((_featureList[i].featureId - 100) / MAX_RANK_NUM) / 2;
            isEnable[AD_RANK_NUM*featureType + rankType] = 1;
        }
    }

    for (uint32_t i = 0; i < RANK_NUM; i++)
    {
        if (isEnable[i] == 1)
        {
            LoadRankCtr(modelPath.c_str(), rankFile[i], _rank2ctr[i], _rank2id[i]);
        }
    }

    return 0;
}

int CRankFeature::GetPos(float rank) const
{
    if (rank < 0)
    {
        return RANK_RANGE - 1;
    }
    if (rank > 100)
    {
        return RANK_RANGE - 2;
    }

    return uint32_t(rank * 10);
}

int CRankFeature::LoadRankCtr(const char *modelPath, const char *fileName, vector<float> &rank2ctr, vector<uint32_t> &rank2id)
{
    string fullFileName = GetFullPath(modelPath, fileName);

    CRegDataCenter::Instance()->LoggingNormal("Loading %s ...", fullFileName.c_str());

    ifstream ifs(fullFileName.c_str());
	if (!ifs)
		throw;
    
    string line;
    rank2ctr.resize(RANK_RANGE);
    rank2id.resize(RANK_RANGE);

    uint32_t currentId = 0;
    float currentCtr = 0.0f;
    uint32_t currentRank = 0;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() < 4)
        {
            CRegDataCenter::Instance()->LoggingError("load file %s error", fileName);
            continue;
        }

        vector<string> ranges;
        SplitString(elements[0], ',', ranges, false);
        if (ranges.size() != 2)
        {
            CRegDataCenter::Instance()->LoggingError("load file %s error", fileName);
            continue;            
        }
        
        currentCtr = atof(elements[3].c_str());
        currentId++;

        if (ranges[1] == "")
        {
      
            float rank = atof(ranges[0].c_str());
            rank2ctr[GetPos(rank)] = currentCtr;
            rank2id[GetPos(rank)] = currentId;
        }
        else
        {
            uint32_t endRank = GetPos(atof(ranges[1].c_str()));
            for (uint32_t i = currentRank; i < endRank; i++)
            {
                if (rank2id[i] == 0)
                {
                    rank2ctr[i] = currentCtr;
                    rank2id[i] = currentId;
                }
            }
            currentRank = endRank;
        }
    }
    for (uint32_t i = currentRank; i < RANK_RANGE - 1; i++)
    {
        if (rank2id[i] == 0)
        {
            rank2ctr[i] = currentCtr;
            rank2id[i] = currentId;
        }
    }
  
    return 0;
}


int CRankFeature::GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    float rankValues[RANK_NUM] = {
        oneCase.rankInfo.absolute,
        oneCase.rankInfo.title,
        oneCase.rankInfo.desc,         
        oneCase.rankInfo.titleHighlight,
        oneCase.rankInfo.descHighlight,
        oneCase.rankInfo.matchCover,   
        oneCase.rankInfo.onlyMatch,    
        oneCase.rankInfo.onlyNonmatch, 
        oneCase.rankInfo.adExtension,  
        oneCase.rankInfo.concurrence,

		oneCase.webRankInfo.web_fixed_rank,
		oneCase.webRankInfo.web_content_rank,
		oneCase.webRankInfo.web_key_bm25_rank,
		oneCase.webRankInfo.web_bm25_rank,
		oneCase.webRankInfo.web_match_rank,
		oneCase.webRankInfo.web_page_rank,
		oneCase.webRankInfo.web_user_rank,
		oneCase.webRankInfo.web_time_rank,
		oneCase.webRankInfo.web_rank,
		oneCase.webRankInfo.web_final_rank
    };  

    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            IdWeight featureResult;

            uint32_t featureType = ((_featureList[i].featureId - 100) / MAX_RANK_NUM) / 2;
            uint32_t rankType = _featureList[i].featureId % MAX_RANK_NUM + AD_RANK_NUM*featureType;
            featureType = ((_featureList[i].featureId - 100) / MAX_RANK_NUM) % 2;
            float rankValue = rankValues[rankType];
            uint32_t rankPos = GetPos(rankValue);

            switch (featureType)
            {
            case RANK_FEATURE_MULTINOMIAL:
                featureResult.id = _featureList[i].startOffset;
                featureResult.weight = _rank2ctr[rankType][rankPos];
                break;
            case RANK_FEATURE_BINARY:
                featureResult.id = _featureList[i].startOffset + _rank2id[rankType][rankPos];
                featureResult.weight = 1;

                break;
            }

#ifdef FEATURE_DEBUG
            LogFeatureInfo(_featureList[i], featureResult, "%f", rankValue);
#endif
            features.push_back(featureResult);
        }
    }

    return 0;
}

int CRankFeature::GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    return 0;
}

