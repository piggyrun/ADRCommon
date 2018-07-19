#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include "RegressionCommon.h"

int ParseRegressionData(const string &line,  SQueryInfo &queryInfo, EAppScenario scenario = SCENARIO_WEB);

void ParsePvHour(SQueryInfo &queryInfo);

void ParseCategory(SQueryInfo &queryInfo);

void ProcessOfflineBehavior(SQueryInfo &queryInfo);

void ProcessTermInfo(SQueryInfo &queryInfo);

int LoadCtrMap(const char * modelPath, const char* fileName, Int2FloatHash &dataMap, size_t bucketSize, bool isMd5);

int LoadListMap(const char* modelPath, const char* fileName, Int2IntHash &dataMap, size_t bucketSize, bool isMd5);

int LoadSiteRank(const char* modelPath, const char* fileName, Long2IntHash &dataMap, size_t bucketSize, bool isMd5);

int LoadCtrBinaryMap(const char * modelPath, const char* fileName, Int2IntHash &dataMap, size_t bucketSize, bool isMd5);

int ConvertPvClick2id(uint32_t pv, uint32_t click, float ctr);

int LoadCtrPidMap(const char* modelPath, const char* fileName, Int2IntHash &dataMap, size_t bucketSize);
 
string GetSite(const string &url);

#endif
