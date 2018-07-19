#ifndef REGRESSION_H
#define REGRESSION_H

#include "FeatureManager.h"
#include "RegressionCommon.h"

class CRegression
{
public:
    CRegression(bool isOnline, uint32_t implementation = IMPLEMENT_ALL, EAppScenario scenario = SCENARIO_WEB);

    CRegression(bool isOnline, const string &featureFile, uint32_t implementation = IMPLEMENT_ALL, EAppScenario scenario = SCENARIO_WEB);

    CRegression(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL, EAppScenario scenario = SCENARIO_WEB);


    int Initialize(const string &modelPath);

    void SetLogFunc(FuncPtr funcPtr);
    
    void SetErrorFunc(FuncPtr funcPtr);

    void GenerateTrainingData(const string &inputFile, const string &outputFile, uint32_t adtype, bool isFirstOnly = true, bool isTest = false);

	void GetTermInfo(SQueryInfo &oneCase); // interface to do word segmentation of web docs

    void Predict(const string &inputFile, const string &modelFile, uint32_t model_num, const string &outputFile);

    void GenFeatureList();

	// !!!暂时这么使用：根据特征取值，获取用哪一个模型
	uint32_t GetModelNum(const vector<IdWeight> &features) const;

    int LoadRegressionCoef(const string &filePath, uint32_t model_num = 0);

    void CalcRegressionMiddleResult(vector<IdWeight> &features, float &currentResult, uint32_t model_num = 0);

    void CalcRegressionMiddleResult(const SQueryInfo &oneCase, float &currentResult);

    float CalcFinalCtr(float currentResult, uint32_t model_num = 0);

    float CalcCtr(vector<IdWeight> &features, uint32_t model_num = 0);

    float CalcCtr(const SQueryInfo &oneCase);

    ~CRegression();
private:
	EAppScenario _scenario; // model applied scenario
    CFeatureManager _featureManager;

    //vector<float> _coefs;
    vector<float> _coefs_array[MAX_MODEL_NUM]; // 各个模型的系数
};

#endif
