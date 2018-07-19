#ifndef REG_DATA_CENTER_H
#define REG_DATA_CENTER_H

#include <string>
#include <map>
#include <ext/hash_map>
using namespace std;
using namespace __gnu_cxx;

#include "RegressionCommon.h"

typedef void (*FuncPtr)(const char*) ;
struct SPvInfo
{
    SPvInfo()
    {
        time=0;
        click=0;
        category=0;
        adid=0;
    }
    uint32_t time;
    uint32_t click;
    uint32_t category;
    uint32_t adid;
};

// 输入经过分词后的结果
struct t_segment_res
{
	uint32_t id;          ///< 该term的termid
	uint8_t pos;         ///< 在输入中的起始位置
	uint8_t len;         ///< term长度
};

typedef map<string, vector<SPvInfo> > UserMap;

class CRegDataCenter
{
public:
    static CRegDataCenter* Instance();

    int Initialize(const string &modelPath, EAppScenario scenario = SCENARIO_WEB);

    void Release();

    bool IsLoaded()
    {
        return _isLoaded;
    }

    void SetLogFunc(FuncPtr funcPtr);
    
    void SetErrorFunc(FuncPtr funcPtr);

    void LoggingNormal(const char *content, ...) const;

    void LoggingError(const char *content, ...) const;
        
	int do_segment(const char * query, vector<t_segment_res> &seg_res) const;

    uint32_t GetCateIndex(uint32_t cate) const;
    uint32_t GetCateIndexHigh(uint32_t cate) const;

    uint32_t GetCateCount() const
    {
        return _cateCount;
    }
    uint32_t GetCateCountHigh() const
    {
        return _cateCountHigh;
    }

    void SetAvgCtr(float ctr)
    {
        _avgCtr = ctr;
    }

    float GetAvgCtr()
    {
        return _avgCtr;
    }

protected:
    CRegDataCenter();

private:
    int LoadCate2Id(const string &modelPath, const char *fileName, Int2IntHash &indexMap, uint32_t &count);

    uint32_t GetIndex(uint32_t cate, const Int2IntHash &indexMap) const;


private:
    bool _isLoaded;
    static CRegDataCenter* _instance;

    FuncPtr _logFunc;
    FuncPtr _errorFunc;

    Int2IntHash  _cate2index;
    Int2IntHash  _cate2indexHigh;

    uint32_t _cateCount;
    uint32_t _cateCountHigh;

    float _avgCtr;

public:
    UserMap _user2pvs;

};

#endif
