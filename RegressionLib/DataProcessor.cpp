#include <time.h>
#include <fstream>
#include <iostream>
#include "DataProcessor.h"
#include "RegDataCenter.h"

uint32_t ParseIp(const string &ipstring)
{
    vector<string> addr;
    SplitString(ipstring, '.', addr, false);
    if (addr.size() != 4 )
    {
        return 0;
    }
    return (atoi(addr[0].c_str()) << 24) + (atoi(addr[1].c_str()) << 16) + (atoi(addr[2].c_str()) << 8) + atoi(addr[3].c_str());
    
}

void ParsePvHour(SQueryInfo &queryInfo)
{
    struct tm *pvtime;
    pvtime = localtime(&queryInfo.visitInfo.pvtime);
    queryInfo.visitInfo.pvhour = pvtime->tm_hour;
}

void ParseCategory(SQueryInfo &queryInfo)
{
    queryInfo.queryAdInfo.categoryIndex = CRegDataCenter::Instance()->GetCateIndex(queryInfo.queryAdInfo.category);
    queryInfo.queryAdInfo.categoryHigh = CRegDataCenter::Instance()->GetCateIndexHigh(queryInfo.queryAdInfo.category);

}

void ProcessSetence(const string &sentence, vector<uint32_t> &processedVec)
{
    processedVec.clear();
    vector<string> elements;
    SplitString(sentence, ',', elements, false);
    processedVec.reserve(elements.size());
    for (size_t i = 0; i < elements.size(); i++)
    {
        processedVec.push_back(atoi(elements[i].c_str()));
    }

}

int ParseRegressionData(const string &line,  SQueryInfo &queryInfo, EAppScenario scenario)
{
    vector<string> elements;
	switch (scenario)
	{
		case SCENARIO_AD:
			SplitString(line, '\t', elements, false);
			if (elements.size() < 48)
				return -1;
			break;
		case SCENARIO_WEB:
			SplitString(line, '', elements, false);
			if (elements.size() < 20)
				return -1;
			break;
		default:
			return -1;
	}

	switch (scenario)
	{
		case SCENARIO_AD:
			queryInfo.adtype    = atoi(elements[5].c_str());
			if (queryInfo.adtype == 0)
			{
				queryInfo.adtype = TYPE_LEFT;
			}
			else
			{
				queryInfo.adtype = TYPE_GROUND;
			}

			queryInfo.adcount   = atoi(elements[12].c_str());
			queryInfo.position  = atoi(elements[15].c_str());
			queryInfo.isClick   = atoi(elements[16].c_str());

			queryInfo.queryAdInfo.query     = elements[0];
			queryInfo.queryAdInfo.adid      = atoi(elements[1].c_str());
			queryInfo.queryAdInfo.accountid = atoi(elements[2].c_str());
			queryInfo.queryAdInfo.category  = atoi(elements[3].c_str());   

			queryInfo.visitInfo.locale      = atoi(elements[7].c_str());
			queryInfo.visitInfo.pvtime      = atoi(elements[10].c_str());
			queryInfo.visitInfo.suid        = elements[8];
			queryInfo.visitInfo.ip          = ParseIp(elements[6].c_str());
			queryInfo.visitInfo.isHint      = atoi(elements[13].c_str());
			queryInfo.visitInfo.isSuggestion= atoi(elements[14].c_str());
			queryInfo.visitInfo.pid         = elements[4];
			queryInfo.visitInfo.channel       = 0;

			queryInfo.rankInfo.relevance    = atof(elements[18].c_str());
			queryInfo.rankInfo.content      = atof(elements[20].c_str());
			queryInfo.rankInfo.absolute     = atof(elements[22].c_str());
			queryInfo.rankInfo.matchFeature = atof(elements[23].c_str());
			queryInfo.rankInfo.matchCover   = atof(elements[24].c_str());
			queryInfo.rankInfo.adExtension  = atof(elements[25].c_str());
			queryInfo.rankInfo.urlExtension = atof(elements[26].c_str());
			queryInfo.rankInfo.concurrence  = atof(elements[27].c_str());
			queryInfo.rankInfo.keyword      = atof(elements[30].c_str());
			queryInfo.rankInfo.title        = atof(elements[31].c_str());
			queryInfo.rankInfo.desc         = atof(elements[32].c_str());
			queryInfo.rankInfo.titleHighlight= atof(elements[33].c_str());
			queryInfo.rankInfo.descHighlight= atof(elements[34].c_str());
			queryInfo.rankInfo.onlyMatch    = atof(elements[35].c_str());
			queryInfo.rankInfo.onlyNonmatch = atof(elements[36].c_str());
			queryInfo.rankInfo.matchNonmatch= atof(elements[37].c_str());


			ProcessSetence(elements[38], queryInfo.termInfo.originalTerms);
			ProcessSetence(elements[39], queryInfo.termInfo.mergedTerms);
			ProcessSetence(elements[40], queryInfo.termInfo.mergedTermWeights);
			ProcessSetence(elements[41], queryInfo.termInfo.processedTerms);
			ProcessSetence(elements[42], queryInfo.termInfo.processedTermWeights);
			queryInfo.termInfo.queryTermValue = atoi(elements[43].c_str());
			ProcessSetence(elements[44], queryInfo.termInfo.keyTerms);
			ProcessSetence(elements[45], queryInfo.termInfo.titleTerms);
			ProcessSetence(elements[46], queryInfo.termInfo.descTerms);
			queryInfo.termInfo.keyTermValue = atoi(elements[47].c_str());

			ParsePvHour(queryInfo);
			ParseCategory(queryInfo);

			break;

		case SCENARIO_WEB:
			queryInfo.position  = atoi(elements[7].c_str());
			queryInfo.isClick   = atoi(elements[0].c_str());

			queryInfo.queryDocInfo.query = elements[1];
			queryInfo.queryDocInfo.title = elements[9];
			queryInfo.queryDocInfo.url = elements[8];
			queryInfo.queryDocInfo.site = GetSite(queryInfo.queryDocInfo.url);
			queryInfo.queryDocInfo.flag = atol(elements[19].c_str());   

			// normailize all rank to 0~100
			queryInfo.webRankInfo.web_fixed_rank = atof(elements[10].c_str()) * 100;
			queryInfo.webRankInfo.web_content_rank = atof(elements[11].c_str());
			queryInfo.webRankInfo.web_key_bm25_rank = atof(elements[12].c_str()) * 10;
			queryInfo.webRankInfo.web_bm25_rank = atof(elements[13].c_str());
			queryInfo.webRankInfo.web_match_rank = atof(elements[14].c_str());
			uint16_t pu_rank = atoi(elements[15].c_str());
			queryInfo.webRankInfo.web_page_rank = (pu_rank/256) * 100.0f/256;
			queryInfo.webRankInfo.web_user_rank = (pu_rank%128) * 100.0f/128;
			queryInfo.webRankInfo.web_time_rank = atof(elements[16].c_str()) * 10;
			queryInfo.webRankInfo.web_rank = atof(elements[17].c_str()) * 100.0f/2000;
			queryInfo.webRankInfo.web_final_rank = atof(elements[18].c_str()) * 10;

			break;
	}

    return 0;
}

void ProcessOfflineBehavior(SQueryInfo &queryInfo)
{
    SPvInfo pvinfo;

    pvinfo.time = queryInfo.visitInfo.pvtime;
    pvinfo.click = queryInfo.isClick;
    pvinfo.category = queryInfo.queryAdInfo.category;
    pvinfo.adid = queryInfo.queryAdInfo.adid;    

    uint32_t featureId = 0;

    string suid = queryInfo.visitInfo.suid;

    UserMap::iterator iter = CRegDataCenter::Instance()->_user2pvs.find(suid);

    if (iter == CRegDataCenter::Instance()->_user2pvs.end())
    {
        vector<SPvInfo> newUser;
        CRegDataCenter::Instance()->_user2pvs.insert(pair<string, vector<SPvInfo> >(suid, newUser));
        iter = CRegDataCenter::Instance()->_user2pvs.find(suid);
    }

    if (queryInfo.position == 1 || iter->second.size() == 0)
    {
        iter->second.push_back(pvinfo);    
    }

    uint32_t hasClick = 0;
    uint32_t pv = 0;
    uint32_t hasCategory = 0;
    uint32_t hasAdid = 0;
    for (uint32_t i = 0; i < iter->second.size() - 1; i++)
    {
        if (pvinfo.time - iter->second[i].time <= 3600)
        {
            pv++;
            if (pvinfo.category == iter->second[i].category)
            {
                hasCategory = 1;
            }

            if (pvinfo.adid == iter->second[i].adid)
            {
                hasAdid = 1;
            }


            if (iter->second[i].click == 1)
            {
                hasClick = 1;
                if (pvinfo.time - iter->second[i].time <= 600)
                {
                    hasClick = 2;
                }
                if (pvinfo.time - iter->second[i].time <= 300)
                {
                    hasClick = 3;
                }
                if (pvinfo.time - iter->second[i].time <= 60)
                {
                    hasClick = 4;
                }

            }
        }
    }
    if (hasClick != 0)
    {
        featureId = 80 + hasClick;
    } else
    {
        if (pv > 100)
        {
            pv = 12;
        }
        else if (pv > 30)
        {
            pv = 11;
        }
        else if (pv > 10)
        {
            pv = 10;
        }

        if (hasAdid == 1)
        {
            featureId = pv + 40;
        }
        else
        {
            featureId = pv + hasCategory * 20;
        }
    }
    queryInfo.behavior = featureId;
}

int LoadCtrMap(const char* modelPath, const char* fileName, Int2FloatHash &dataMap, size_t bucketSize, bool isMd5)
{
    string fullPath = GetFullPath(modelPath, fileName);

    CRegDataCenter::Instance()->LoggingNormal("Loading %s ...", fullPath.c_str());

    dataMap.open(bucketSize);

    ifstream ifs(fullPath.c_str());
	if (!ifs)
	{
		fprintf(stderr, "load %s failed\n", fileName);
		return -1;
	}
    string line;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() != 4)
        {
            continue;
        }
        uint32_t key;
        if (!isMd5)
        {
            key = atoi(elements[0].c_str());
        }
        else
        {
            md5_long_32 md5_output = getSign32(elements[0].c_str(), -1);
            key = md5_output.data.intData[0];            
        }
        float ctr = atof(elements[3].c_str());

        dataMap.bind(key, ctr);
        
    }

    return 0;
}

int LoadCtrBinaryMap(const char* modelPath, const char* fileName, Int2IntHash &dataMap, size_t bucketSize, bool isMd5)
{
    string fullPath = GetFullPath(modelPath, fileName);

    CRegDataCenter::Instance()->LoggingNormal("Loading %s ...", fullPath.c_str());

    dataMap.open(bucketSize);

    ifstream ifs(fullPath.c_str());
	if (!ifs)
		return -1;

    string line;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() != 4)
        {
            continue;
        }
        uint32_t key;
        if (!isMd5)
        {
            key = atoi(elements[0].c_str());
        }
        else
        {
            md5_long_32 md5_output = getSign32(elements[0].c_str(), -1);
            key = md5_output.data.intData[0];            
        }
        uint32_t pv = atoi(elements[1].c_str());
        uint32_t click = atoi(elements[2].c_str());
        float ctr = atof(elements[3].c_str());

        uint32_t id = ConvertPvClick2id(pv, click, ctr);
        dataMap.bind(key, id);
//        printf("%d\t%d\t%f\t%d\n", pv, click, ctr, id);
        
    }
    return 0;
}

//const uint32_t PV_TABLE[] = {1, 3, 6, 9, 15, 20, 30, 50, 80};
//const uint32_t CLICK_TABLE[] = {0, 1, 2, 3, 6, 10};
//const float CTR_TABLE[] = {0.01, 0.02, 0.04, 0.06, 0.10, 0.15, 0.25, 0.40, 0.60, 0.80};


const uint32_t PV_TABLE[] = {1, 2, 3, 4, 5, 6, 8, 11, 14, 18, 23, 29, 36, 44, 54, 66, 80};
const uint32_t CLICK_TABLE[] = {0, 1, 2, 3, 4, 5, 6, 8, 10};
const float CTR_TABLE[] = {0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.07, 0.08, 0.09, 0.10, 
                           0.12, 0.14, 0.16, 0.18, 0.20, 0.22, 0.25, 0.30, 0.35, 0.40, 0.50, 0.60, 1.00};


int ConvertPvClick2id(uint32_t pv, uint32_t click, float ctr)
{
    uint32_t pvSize = sizeof(PV_TABLE) / sizeof(uint32_t);
    uint32_t clickSize = sizeof(CLICK_TABLE) / sizeof(uint32_t);

    uint32_t pvIndex = 0, clickIndex = 0;
    
    while (pvIndex < pvSize && pv > PV_TABLE[pvIndex])
    {
        pvIndex++;
    }

    while (clickIndex < clickSize && click > CLICK_TABLE[clickIndex])
    {
        clickIndex++;
    }

    uint32_t id = 0;
    if (pvIndex == pvSize || clickIndex == clickSize)
    {
        uint32_t ctrSize = sizeof(CTR_TABLE) / sizeof(uint32_t);
        uint32_t ctrIndex = 0;
        while (ctrIndex < ctrSize && ctr > CTR_TABLE[ctrIndex])
        {
           ctrIndex++;
        }
        id = pvSize * clickSize + ctrIndex + 1;      
    }
    else
    {
        id = pvIndex * clickSize + clickIndex + 1;   
    }
    return id;

}

const string SpecialPids[] = {
    "sogou-site-a5912d5771cbddba",
    "sogou-misc-85bcf11c65e51c2a",
    "sogou-misc-bbebea45772f4ff0",
    "sogou-site-576da016dd73decc",
    "sogou-site-3fb92b8033db6076",
    "sogou-site-f55c452ae38c1e21",
    "sogou-clse-eda5b489bc6d0b7d",
    "sogou-clse-d3e241cdebeed984",
    "sogou-clse-0dc94ddee6d0b5cf",
    "sogou-clse-f26a1be24a9ff7bb"
};
    



int LoadCtrPidMap(const char* modelPath, const char* fileName, Int2IntHash &dataMap, size_t bucketSize)
{
    string fullPath = GetFullPath(modelPath, fileName);

    CRegDataCenter::Instance()->LoggingNormal("Loading %s ...", fullPath.c_str());

    dataMap.open(bucketSize);

    size_t specialSize = sizeof(SpecialPids) / sizeof(string);

    ifstream ifs(fullPath.c_str());
	if (!ifs)
		return -1;

    string line;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() != 4)
        {
            continue;
        }

        uint32_t pv = atoi(elements[1].c_str());
        //uint32_t click = atoi(elements[2].c_str());
        float ctr = atof(elements[3].c_str());



        uint32_t key;
        md5_long_32 md5_output = getSign32(elements[0].c_str(), -1);
        key = md5_output.data.intData[0];            

        
        uint32_t featureId = 0;
        for (uint32_t i = 0; i < specialSize; i++)
        {
            if (SpecialPids[i] == elements[0])
            {
                featureId = i + 50;
                break;
            }
        }
        if (featureId == 0)
        {
            uint32_t ctrSize = sizeof(CTR_TABLE) / sizeof(uint32_t);
            uint32_t ctrIndex = 0;
            while (ctrIndex < ctrSize && ctr > CTR_TABLE[ctrIndex])
            {
               ctrIndex++;
            }
            if (pv > 50)
            {
                featureId = ctrIndex + 1;
            }
            
        }
        dataMap.bind(key, featureId);
        //printf("%s\t%d\t%d\t%f\t%d\n", elements[0].c_str(), pv, click, ctr, featureId);
        
    }
    return 0;
}

string GetSite(const string &url)
{
	string site;
	size_t start = url.find("://");
	start += 3;
	if (start >= url.size())
		return site;
	size_t end = url.find("/", start);
	site = url.substr(start, end-start);
	return site;
}

void ProcessTermInfo(SQueryInfo &queryInfo)
{
	vector<t_segment_res> seg_res;
	if (CRegDataCenter::Instance()->do_segment(queryInfo.queryDocInfo.query.c_str(), seg_res) == 0)
	{
		queryInfo.termInfo.originalTerms.clear();
		for (size_t i=0; i<seg_res.size(); i++)
		{
			queryInfo.termInfo.originalTerms.push_back(seg_res[i].id);
		}
	}
	if (queryInfo.queryDocInfo.title.size() > 0)
	{
		if (CRegDataCenter::Instance()->do_segment(queryInfo.queryDocInfo.title.c_str(), seg_res) == 0)
		{
			queryInfo.termInfo.titleTerms.clear();
			for (size_t i=0; i<seg_res.size(); i++)
			{
				queryInfo.termInfo.titleTerms.push_back(seg_res[i].id);
			}
		}
	}
}

int LoadListMap(const char* modelPath, const char* fileName, Int2IntHash &dataMap, size_t bucketSize, bool isMd5)
{
    string fullPath = GetFullPath(modelPath, fileName);
    CRegDataCenter::Instance()->LoggingNormal("Loading %s ...", fullPath.c_str());
    dataMap.open(bucketSize);

    ifstream ifs(fullPath.c_str());
	if (!ifs)
	{
		fprintf(stderr, "load %s failed\n", fileName);
		return -1;
	}
    string line;
	uint32_t index = 0;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() < 1)
        {
            continue;
        }
        uint32_t key;
        if (!isMd5)
        {
            key = atoi(elements[0].c_str());
        }
        else
        {
            md5_long_32 md5_output = getSign32(elements[0].c_str(), -1);
            key = md5_output.data.intData[0];            
        }
        dataMap.bind(key, index);
		index++;
    }

    return 0;
}

int LoadSiteRank(const char* modelPath, const char* fileName, Long2IntHash &dataMap, size_t bucketSize, bool isMd5)
{
    string fullPath = GetFullPath(modelPath, fileName);
    CRegDataCenter::Instance()->LoggingNormal("Loading %s ...", fullPath.c_str());
    dataMap.open(bucketSize);

    ifstream ifs(fullPath.c_str());
	if (!ifs)
	{
		fprintf(stderr, "load %s failed\n", fileName);
		return -1;
	}
    string line;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, '\t', elements, false);
        if (elements.size() < 2)
        {
            continue;
        }
        uint64_t key;
        if (!isMd5)
        {
            key = atol(elements[0].c_str());
        }
        else
        {
            md5_long_64 md5_output = getSign64(elements[0].c_str(), -1);
            key = md5_output.data.intData[0];            
        }
		uint32_t value = atoi(elements[1].c_str());
        dataMap.bind(key, value);
    }

    return 0;
}
