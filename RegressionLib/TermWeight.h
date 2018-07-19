/**
 * @Author  Shi Feng 
 * @Date  2010-8-24
 * @Brief 管理Term Weight数据
 */


#ifndef TERM_WEIGHT_H
#define TERM_WEIGHT_H

#include <string>
using namespace std;

#include <ace/Null_Mutex.h>
#include <ace/Hash_Map_Manager_T.h>


typedef ACE_Hash_Map_Manager_Ex<uint32_t, float, ACE_Hash< uint32_t >, ACE_Equal_To< uint32_t >, ACE_Null_Mutex> t_subject_value_map;

typedef ACE_Hash_Map_Manager_Ex<uint32_t, uint32_t, ACE_Hash<uint32_t>, ACE_Equal_To<uint32_t>, ACE_Null_Mutex> t_commercial_weight_map;

class CTermWeight
{
public:
    static CTermWeight* Instance();

    int Initialize(const string &modelPath);

    int LoadSubjectWeightValue(const string &modelPath);

    int LoadCommercialWeightValue(const string & modelPath);

    void Release();

    bool IsLoaded()
    {
        return _isSubjectLoaded && _isCommercialLoaded;
    }


        
    float GetSubjectWeight(uint32_t wordId);

    uint32_t GetCommercialWeight(uint32_t wordId);

    uint32_t GetMixWeight(uint32_t wordId, float weight1 = 0.55, float weight2 = 0.45);

protected:
    CTermWeight();

private:
    bool _isSubjectLoaded;
	bool _isCommercialLoaded;
    static CTermWeight* _instance;

    t_subject_value_map subject_value_map;
    t_commercial_weight_map commercial_value_map;

};


#endif
