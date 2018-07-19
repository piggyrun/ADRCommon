#include <fstream>
#include <sstream>
#include "Regression.h"
#include "RegDataCenter.h"
#include "DataProcessor.h"

const char* DefaultFeatureFile = "FeatureDefault";
const char* AllFeatureFile     = "FeatureAll";

CRegression::CRegression(bool isOnline, uint32_t implementation, EAppScenario scenario)
    :_featureManager(isOnline, implementation)
{
	_scenario = scenario;
}

CRegression::CRegression(bool isOnline, const string &featureFile, uint32_t implementation, EAppScenario scenario)
    :_featureManager(isOnline, featureFile, implementation)
{
	_scenario = scenario;
}

CRegression::CRegression(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation, EAppScenario scenario)
    :_featureManager(isOnline, useFeatureList, implementation)
{
	_scenario = scenario;
}


int CRegression::Initialize(const string &modelPath)
{
	// initialize data center
	CRegDataCenter::Instance()->LoggingNormal("Initializing Data Center...");
	CRegDataCenter::Instance()->Initialize(modelPath, _scenario);
	CRegDataCenter::Instance()->LoggingNormal("Initialize Data Center ok.");

	// initialize feature manager
    CRegDataCenter::Instance()->LoggingNormal("Initializing Feature Manager...");
    _featureManager.Initialize(modelPath);
    CRegDataCenter::Instance()->LoggingNormal("Initialize Feature Manager ok.");

    return 0;
}

void CRegression::GenFeatureList()
{
    CRegDataCenter::Instance()->LoggingNormal("Generate default features to %s ...", DefaultFeatureFile);
    _featureManager.OutputFeatureList(DefaultFeatureFile, false);
    CRegDataCenter::Instance()->LoggingNormal("Generate all features to %s ...", AllFeatureFile);
    _featureManager.OutputFeatureList(AllFeatureFile, true);
}

void CRegression::SetLogFunc(FuncPtr funcPtr)
{
    CRegDataCenter::Instance()->SetLogFunc(funcPtr);
}

void CRegression::SetErrorFunc(FuncPtr funcPtr)
{
    CRegDataCenter::Instance()->SetErrorFunc(funcPtr);
}

int CRegression::LoadRegressionCoef(const string &filePath, uint32_t model_num)
{
    CRegDataCenter::Instance()->LoggingNormal("Loading coef file : %s for model No. %u", filePath.c_str(), model_num);
	if (model_num >= MAX_MODEL_NUM)
		return -1;

    ifstream ifs(filePath.c_str());
    if (ifs == NULL)
    {
        return -1;
    }
    string line;
	vector<float> &_coefs = _coefs_array[model_num];
    
    string infos[6];
    for (uint32_t i = 0; i < 6; i++)
    {
        if (!getline(ifs, infos[i]))
        {
            return -1;
        }
    }

    float multi = 1.0f;
    uint32_t featureCount = 0;
    uint32_t tempInt = 0;
    string tempStr = "";    

    istringstream iss(infos[2]);
    iss >> tempStr >> tempInt;
    if (tempInt == 0)
    {
        multi = -1.0f;
    }

    istringstream iss2(infos[3]);
    iss2 >> tempStr >> featureCount;
    _coefs.resize(featureCount + 1);

    CRegDataCenter::Instance()->LoggingNormal("Feature Count : %d\tmulti : %f", featureCount, multi);

    for (uint32_t i = 0; i < featureCount; i++)
    {
        if (!getline(ifs, line))
        {
            return -1;
        }
        _coefs[i + 1] = atof(line.c_str()) * multi;
    }

    if (!getline(ifs, line))
    {
        return -1;
    }
    
    _coefs[0] = atof(line.c_str()) * multi;


    CRegDataCenter::Instance()->LoggingNormal("intercept : %f", _coefs[0]);
    ifs.close();
    return 0;
}

void CRegression::CalcRegressionMiddleResult(vector<IdWeight> &features, float &currentResult, uint32_t model_num)
{
	vector<float> &_coefs = _coefs_array[model_num];
    for (size_t i = 0; i < features.size(); i++)
    {
        if (features[i].id >= _coefs.size())
        {
            continue;
        }
        currentResult += _coefs[features[i].id] * features[i].weight;
        //printf("%d\t%f\t%f\t%f\n", features[i].id, _coefs[features[i].id], features[i].weight, currentResult);
    }
}


void CRegression::CalcRegressionMiddleResult(const SQueryInfo &oneCase, float &currentResult)
{
    vector<IdWeight> features;
    _featureManager.GetFeatures(oneCase, features);
	uint32_t model_num = GetModelNum(features);
    CalcRegressionMiddleResult(features, currentResult, model_num);
}

float CRegression::CalcFinalCtr(float currentResult, uint32_t model_num)
{
	vector<float> &_coefs = _coefs_array[model_num];
    return 1.0f / (1 + exp(-(currentResult + _coefs[0])));
}

float CRegression::CalcCtr(vector<IdWeight> &features, uint32_t model_num)
{
    float middleResult = 0.0f;

    CalcRegressionMiddleResult(features, middleResult, model_num);
    return CalcFinalCtr(middleResult, model_num);
}

float CRegression::CalcCtr(const SQueryInfo &oneCase)
{
    vector<IdWeight> features;
    _featureManager.GetFeatures(oneCase, features);
	uint32_t model_num = GetModelNum(features);

    return CalcCtr(features, model_num);
}

void CRegression::Predict(const string &inputFile, const string &modelFile, uint32_t model_num, const string &outputFile)
{
    if (LoadRegressionCoef(modelFile, model_num) != 0)
    {
        return;
    }
    ifstream ifs(inputFile.c_str());
    ofstream ofs(outputFile.c_str());
    string line;
    uint32_t count = 0;
    while (getline(ifs, line))
    {
        vector<string> elements;
        SplitString(line, ' ', elements, false);        
        //SQueryInfo info;
        //ParseRegressionData(line, info);
        //ProcessOfflineBehavior(info);

        //if (info.adtype != adtype)
        //{
        //    continue;
        //}

        vector<IdWeight> features;

        for (uint32_t i = 1; i < elements.size(); i++)
        {
            vector<string> featureVec;
            SplitString(elements[i], ':', featureVec, false);
            IdWeight newFeature;
            newFeature.id = atoi(featureVec[0].c_str());
            newFeature.weight = atof(featureVec[1].c_str());
            features.push_back(newFeature);
        }

        float ctr = CalcCtr(features, model_num);

        ofs << elements[0] << " " << ctr << endl;

        count++;
        if (count % 10000 == 0)
        {
            cout << count << endl;
        }

    }
    ofs.close();
    ifs.close();
}



void CRegression::GenerateTrainingData(const string &inputFile, const string &outputFile, uint32_t adtype, bool isFirstOnly, bool isTest)
{
    ifstream ifs(inputFile.c_str());
    ofstream ofs(outputFile.c_str());
	if (!ifs || !ofs)
	{
		CRegDataCenter::Instance()->LoggingNormal("open sample file failed in: %s, out: %s ...", inputFile.c_str(), outputFile.c_str());
		throw;
	}
    string line;
    uint32_t count = 0;

    vector<SPvClick> featureStat(MAX_FEATURE_INDEX);
    while (getline(ifs, line))
    {
        SQueryInfo info;
        if (ParseRegressionData(line, info, _scenario) != 0)
        {
            continue;
        }
		if (SCENARIO_WEB == _scenario)
		{
			ProcessTermInfo(info); // do word_segmentation for query,title
		}
		if (SCENARIO_AD == _scenario)
		{
			ProcessOfflineBehavior(info);
			if (info.adtype != adtype)
				continue;
		}

        if (isFirstOnly && info.position != 1)
        {
            continue;
        }
		if (isTest)
		{
			info.position = 1; // 作评测时，位置均置为1
		}

        vector<IdWeight> features;
        _featureManager.GetFeatures(info, features);
        sort(features.begin(), features.end(), IdWeightSortId);

        ofs << info.isClick;
        for (uint32_t i = 0; i < features.size(); i++)
        {
            ofs << " " << features[i].id << ":";
            if (features[i].weight == 1.0)
            {
                ofs << "1";
            }
            else
            {
                ofs << features[i].weight;
            }
            featureStat[features[i].id].pv += features[i].weight;
            featureStat[features[i].id].click += info.isClick;;

        }
        ofs << endl;
        count++;
        if (count % 10000 == 0)
        {
            cout << count << endl;
            //break;
        }
    }
    ofs.close();
    ifs.close();

    string debugFile = outputFile + ".debug";
    ofstream ofs2(debugFile.c_str());
    for (uint32_t i = 0; i < MAX_FEATURE_INDEX; i++)
    {
        if (featureStat[i].pv != 0)
        {
            ofs2 << i << "\t" << int(featureStat[i].pv) << "\t" << featureStat[i].click << "\t" << featureStat[i].click / featureStat[i].pv << endl;
        }
    }
    ofs2.close();

}


CRegression::~CRegression()
{
}

void CRegression::GetTermInfo(SQueryInfo &oneCase)
{
	ProcessTermInfo(oneCase);
}

uint32_t CRegression::GetModelNum(const vector<IdWeight> &features) const
{
	for (size_t i=0; i<features.size(); i++)
	{
		if (features[i].id >= 34000 && features[i].id < 35000)
		{
			if (features[i].id % 200 != 0) // query_url不充分时用model 1，否则0
				return 0;
		}
	}
	return 1;
}
