#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

#include "TermWeight.h"
#include "RegressionCommon.h"
#include "RegDataCenter.h"

const char* SUBJECT_WEIGHT_FILE = "subjectweight";
const char* COMMERCIAL_WEIGHT_FILE = "term_info.txt";

const uint32_t DEFAULT_COMMERCIAL_VALUE = 8000;

CTermWeight* CTermWeight::_instance = 0;

CTermWeight* CTermWeight::Instance()
{
    if (_instance == 0)
    {
        _instance = new CTermWeight();
    }
    return _instance;
}

CTermWeight::CTermWeight()
	:commercial_value_map(1000000)
{
	
}

int CTermWeight::Initialize(const string &modelPath)
{
    return LoadSubjectWeightValue(modelPath) + LoadCommercialWeightValue(modelPath);
}

int CTermWeight::LoadSubjectWeightValue(const string &modelPath)
{
    ifstream ifs(GetFullPath(modelPath.c_str(), SUBJECT_WEIGHT_FILE).c_str());

    if (ifs == NULL)
    {
		CRegDataCenter::Instance()->LoggingNormal("[regression model term weight feature] open %s failed", SUBJECT_WEIGHT_FILE);
        return -1;
    }

    subject_value_map.unbind_all();

    string line;

    while (getline(ifs, line))
    {
        vector<string> strs;
        SplitString(line, '\t', strs);
        if (strs.size() < 4)
        {
            continue;
        }
        uint32_t termid = atoi(strs[2].c_str());
        uint32_t accountnum = atoi(strs[3].c_str());
        uint32_t categorynum = atoi(strs[4].c_str());
        float value = logf(float(accountnum) + 1.1f) / logf(float(categorynum) + 1.1f);

        subject_value_map.bind(termid, value);
    }
    _isSubjectLoaded = true;

    //fprintf(stderr, "[regression model term weight feature] load subject weight ok\n");
	CRegDataCenter::Instance()->LoggingNormal("[regression model term weight feature] load subject weight ok");

    return 0;
}

int CTermWeight::LoadCommercialWeightValue(const string &modelPath)
{
    ifstream ifs(GetFullPath(modelPath.c_str(), COMMERCIAL_WEIGHT_FILE).c_str());

    if (ifs == NULL)
    {
        //fprintf(stderr, "[regression model term weight feature] open %s failed\n", COMMERCIAL_WEIGHT_FILE);
		CRegDataCenter::Instance()->LoggingNormal("[regression model term weight feature] open %s failed", COMMERCIAL_WEIGHT_FILE);
        return -1;
    }

    commercial_value_map.unbind_all();

    string line;

    while (getline(ifs, line))
    {
        vector<string> strs;
        SplitString(line, '\t', strs);
        if (strs.size() < 3)
        {
            continue;
        }
        uint32_t termid = atoi(strs[0].c_str());
        uint32_t value = atoi(strs[2].c_str());

        commercial_value_map.bind(termid, value);
    }
    _isCommercialLoaded = true;

    //fprintf(stderr, "[regression model term weight feature] load commercial weight ok\n");
	CRegDataCenter::Instance()->LoggingNormal("[regression model term weight feature] load commercial weight ok");
    
	return 0;
}

float CTermWeight::GetSubjectWeight(uint32_t wordId)
{
    float value;
    if (subject_value_map.find(wordId, value) == -1)
    {
        return 0;
    }
    return value;
}

uint32_t CTermWeight::GetCommercialWeight(uint32_t wordId)
{
    uint32_t value;
    if (commercial_value_map.find(wordId, value) == -1)
    {
   // 	fprintf(stderr, "[regression model term weight feature] %u use the default commercial weight ok\n", wordId);
        return DEFAULT_COMMERCIAL_VALUE;
    }
    return value;
}

uint32_t CTermWeight::GetMixWeight(uint32_t wordId, float weight1, float weight2)
{
    return (uint32_t)(GetSubjectWeight(wordId) * 16383 * weight1) + (uint32_t)(GetCommercialWeight(wordId) * weight2);
}
 

