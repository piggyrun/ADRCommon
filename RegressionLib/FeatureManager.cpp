#include "FeatureManager.h"
#include "RankFeature.h"
#include "BiddingFeature.h"
#include "StatFeature.h"
#include "BehaviorFeature.h"
#include "HighlightFeature.h"
#include "TermFeature.h"
#include "LqyFeature.h"
#include "TermWeightFeature.h"
#include "CrossFeature.h"
#include "DocPropFeature.h"
#include "SiteFeature.h"

#include <fstream>


CFeatureManager::CFeatureManager(bool isOnline, uint32_t implementation)
{
    _featureClasses.push_back(new CRankFeature(isOnline, implementation));
    _featureClasses.push_back(new CStatFeature(isOnline, implementation));
    _featureClasses.push_back(new CDocPropFeature(isOnline, implementation));
    _featureClasses.push_back(new CHighlightFeature(isOnline, implementation));
    _featureClasses.push_back(new CSiteFeature(isOnline, implementation));
    //_featureClasses.push_back(new CBiddingFeature(isOnline, implementation));
    //_featureClasses.push_back(new CBehaviorFeature(isOnline, implementation));
    //_featureClasses.push_back(new CTermFeature(isOnline, implementation));
    //_featureClasses.push_back(new CLqyFeature(isOnline, implementation));
    //_featureClasses.push_back(new CTermWeightFeature(isOnline, implementation));
    //_featureClasses.push_back(new CCrossFeature(isOnline, implementation));

    GenerateFeatureInfo();
}

CFeatureManager::CFeatureManager(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation)
{
    _featureClasses.push_back(new CRankFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CStatFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CDocPropFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CHighlightFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CSiteFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CBiddingFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CBehaviorFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CTermFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CLqyFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CTermWeightFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CCrossFeature(isOnline, useFeatureList, implementation));

    GenerateFeatureInfo();
}

CFeatureManager::CFeatureManager(bool isOnline, const string &featureFile, uint32_t implementation)
{
    vector<uint32_t> useFeatureList;

    ifstream ifs(featureFile.c_str());

    if (ifs == NULL)
    {
        CRegDataCenter::Instance()->LoggingError("Open Feature list : %s failed. Initiallize aborted", featureFile.c_str());
        throw;
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
        uint32_t featureId = atoi(elements[0].c_str());
        uint32_t isUse = atoi(elements[1].c_str());
        if (isUse == 1)
        {
            useFeatureList.push_back(featureId);
        }
    }

    _featureClasses.push_back(new CRankFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CStatFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CDocPropFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CHighlightFeature(isOnline, useFeatureList, implementation));
    _featureClasses.push_back(new CSiteFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CBiddingFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CBehaviorFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CTermFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CLqyFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CTermWeightFeature(isOnline, useFeatureList, implementation));
    //_featureClasses.push_back(new CCrossFeature(isOnline, useFeatureList, implementation));

    GenerateFeatureInfo();
}

int CFeatureManager::Initialize(const string &modelPath)
{
    for (uint32_t i = 0; i < _featureClasses.size(); i++)
    {
        CRegDataCenter::Instance()->LoggingNormal("  Initializing %s ...", _featureClasses[i]->GetModuleName().c_str());

        if(_featureClasses[i]->Initialize(modelPath) != 0)
        {
            CRegDataCenter::Instance()->LoggingError("  Initialize %s failed", _featureClasses[i]->GetModuleName().c_str());
            throw;
        }
        else
        {
            CRegDataCenter::Instance()->LoggingNormal("  Initialize %s ok.", _featureClasses[i]->GetModuleName().c_str());
        }
    }
    return 0;
}

void CFeatureManager::OutputFeatureList(const string filePath, bool isAll) const
{
    ofstream ofs(filePath.c_str());
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (isAll || _featureList[i].isUse == FEATURE_ENABLE)
        {
            ofs << _featureList[i].featureId << "\t" << _featureList[i].isUse << "\t" << _featureList[i].featureName << "\t" 
                << _featureList[i].startOffset << "\t" << _featureList[i].endOffset << "\t" 
                << _featureList[i].implementation << "\t" << _featureList[i].featureType << "\t" << endl;
        }
    }
    ofs.close();
}

void CFeatureManager::GenerateFeatureInfo()
{
    for (size_t i = 0; i < _featureClasses.size(); i++)
    {
        vector<SFeatureInfo> newFeatures;
        _featureClasses[i]->GetSupportedFeatures(newFeatures);
        _featureList.insert(_featureList.end(), newFeatures.begin(), newFeatures.end());
    }

    uint32_t useFeatureCount = 0;
    uint32_t isBinary = 1;
    for (size_t i = 0; i < _featureList.size(); i++)
    {
        if (_featureList[i].isUse == FEATURE_ENABLE)
        {
            useFeatureCount++;
            CRegDataCenter::Instance()->LoggingNormal(" Feature : %s", _featureList[i].featureName.c_str());
            if (_featureList[i].featureType != BINARY_FEATURE)
            {
                isBinary = 0;
            }
        }
    }
    if (useFeatureCount == 0)
    {
        CRegDataCenter::Instance()->LoggingNormal("No feature mode");
    }
    else
    {
        CRegDataCenter::Instance()->LoggingNormal("Feature Count : %d", useFeatureCount);
        if (isBinary == 1)
        {
            CRegDataCenter::Instance()->LoggingNormal("All features are BINARY feature");
        }
    }
    

}

CFeatureManager::~CFeatureManager()
{
    for (uint32_t i = 0; i < _featureClasses.size(); i++)
    {
        delete _featureClasses[i];
    }
}

uint32_t CFeatureManager::GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const
{
    features.clear();
    for (uint32_t i = 0; i < _featureClasses.size(); i++)
    {
        _featureClasses[i]->GetFeatures(oneCase, features);
    }
    return 0;
}
