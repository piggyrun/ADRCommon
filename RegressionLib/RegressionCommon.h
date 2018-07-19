#ifndef REGRESSION_COMMON_H
#define REGRESSION_COMMON_H

#include <string>
using namespace std;

#include "../CommonTools.h"
#include "Platform/md5/md5.h"
#include <ace/Null_Mutex.h>
#include <ace/Hash_Map_Manager_T.h>

const uint32_t MAX_MODEL_NUM = 2; // 暂时按是否统计充分，分为两个模型

const uint32_t MAX_FEATURE_INDEX = 10000000;

const uint32_t  TYPE_LEFT       = 1;
const uint32_t  TYPE_GROUND     = 2;

const uint32_t  IMPLEMENT_RANKDOC = 1;
const uint32_t  IMPLEMENT_RERANK  = 2;
const uint32_t  IMPLEMENT_BIDDING = 3;
const uint32_t  IMPLEMENT_ALL     = 0;

const uint32_t  BINARY_FEATURE      = 1;
const uint32_t  MULTINOMIAL_FEATURE = 2;

const uint32_t  FEATURE_ENABLE    = 1;
const uint32_t  FEATURE_DISABLE   = 0;

const uint32_t  LOCALE_COUNT = 34;
const uint32_t  HOUR_COUNT   = 24;

const float     DEFAULT_AVG_CTR = 0.051058;

typedef ACE_Hash_Map_Manager_Ex<uint32_t, float, ACE_Hash< uint32_t >, ACE_Equal_To< uint32_t >, ACE_Null_Mutex> Int2FloatHash;
typedef ACE_Hash_Map_Manager_Ex<uint32_t, uint32_t , ACE_Hash< uint32_t >, ACE_Equal_To< uint32_t >, ACE_Null_Mutex> Int2IntHash;
typedef ACE_Hash_Map_Manager_Ex<uint64_t, uint32_t , ACE_Hash< uint64_t >, ACE_Equal_To< uint64_t >, ACE_Null_Mutex> Long2IntHash;

enum EAppScenario // model apply to ?
{
	SCENARIO_AD = 0,
	SCENARIO_WEB
};

struct SFeatureInfo
{
    string      featureName;
    uint32_t    featureId;
    uint32_t    startOffset;
    uint32_t    endOffset;
    uint32_t    implementation;
    uint32_t    featureType;
    uint32_t    isUse;    
};


struct SQueryAdInfo
{
    string      query;
    uint32_t    adid;
    uint32_t    accountid;
    uint32_t    category;
    uint32_t    categoryIndex;
    uint32_t    categoryHigh;
};

struct SQueryDocInfo
{
	string query;
	string title;
	string url;
	string site;
	uint64_t flag;
};

struct SVisitInfo
{
    uint32_t    locale;
    long        pvtime;
    uint32_t    pvhour;
    string      suid;
    string      yyid;
    uint32_t    ip;
    uint32_t    isHint;
    uint32_t    isSuggestion;
    string      pid;
    uint32_t    channel;
};

// ad ranks
struct SRankInfo
{
    float       relevance;
    float       content;
    float       absolute;
    float       matchFeature;
    float       matchCover;
    float       adExtension;
    float       urlExtension;
    float       concurrence;
    float       keyword;
    float       title;
    float       desc;
    float       titleHighlight;
    float       descHighlight;
    float       onlyMatch;
    float       onlyNonmatch;
    float       matchNonmatch;
};

// web ranks
struct SWebRankInfo
{
	float web_fixed_rank;
	float web_content_rank;
	float web_key_bm25_rank;
	float web_bm25_rank;
	float web_match_rank;
	float web_page_rank;
	float web_user_rank;
	float web_time_rank;
	float web_rank;
	float web_final_rank;
};

struct SAdTermInfo
{
    vector<uint32_t> originalTerms;
    vector<uint32_t> mergedTerms;
    vector<uint32_t> mergedTermWeights;
    vector<uint32_t> processedTerms;
    vector<uint32_t> processedTermWeights;
    uint32_t queryTermValue;
    vector<uint32_t> keyTerms;
    vector<uint32_t> titleTerms;
    vector<uint32_t> descTerms;
    uint32_t keyTermValue;

	vector<uint32_t> title_highlight_pos; // title highlight position list
};

struct SQueryInfo
{
    SQueryAdInfo    queryAdInfo; // ad info
	SQueryDocInfo   queryDocInfo; // web doc info
    SRankInfo       rankInfo;
	SWebRankInfo    webRankInfo; // web doc ranks
    SAdTermInfo     termInfo; // text info
    SVisitInfo      visitInfo;

    uint32_t        adtype; // left ad or grd ad
    uint32_t        adcount;
    uint32_t        position;
    uint32_t        isClick;
    uint32_t        behavior;
};


struct SPvClick
{
    SPvClick() 
    {
        pv = 0.0f;
        click = 0;
    }
    float pv;
    uint32_t click;
};

#endif
