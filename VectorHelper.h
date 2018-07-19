/**
 * @Author  Guo Ang 
 * @Date  2010-4-7
 * @Brief 向量操作函数库
 */

#ifndef VECTOR_HELPER_H
#define VECTOR_HELPER_H

#include <vector>
#include <string>
#include <sstream>
using namespace std;

//#include "CommonTools.h"

//定义IdWeight
struct IdWeight
{
    uint32_t id;
    float weight;
};

//定义密集型IdWeight
struct IdWeightS
{
    unsigned id:24;     
    unsigned weight:8;  
};

//根据id对IdWeight进行排序的比较函数
bool IdWeightSortId(const IdWeight &item1, const IdWeight &item2); 

//根据weight对IdWeight进行排序的比较函数
bool IdWeightSortWeight(const IdWeight &item1, const IdWeight &item2); 


void GetIdWeightArray(vector<IdWeight> &vecInput, uint32_t *idlist, unsigned char *weightList, uint32_t &length);


float DotProduct(const uint32_t *idlist1, const unsigned char *weightList1, uint32_t length1, const uint32_t *idlist2, const unsigned char *weightList2, uint32_t length2);

/**
*      通过数组建立IdWeight向量
*      @param    idlist     [in]    id数组
*      @param    weightList [in]    weight数组
*      @param    length     [in]    数组长度
*      @param    vecResult  [out]   输出向量
*      @return   无
*/    
void BuildVector(const uint32_t *idlist, const unsigned char *weightList, uint32_t length, vector<IdWeight> &vecResult);

/**
*      获取两个IdWeight向量的点积
*      @param    vec1     [in]    向量1
*      @param    vec2     [in]    向量2
*      @return      点积结果
*/    
float DotProduct(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2);


/**
*      获取两个IdWeight向量的求交和点积
*      @param    vec1      [in]    向量1
*      @param    vec2      [in]    向量2
*      @param    vecResult [out]   求交后的结果
*      @return      点积结果
*/    
float VectorJoin(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2, vector<IdWeight> &vecResult);

/**
*      对两个IdWeight向量进行合并
*      @param    vec1      [in]    向量1
*      @param    vec2      [in]    向量2
*      @param    vecResult [out]   求并后的结果
*      @param    weight1   [in]    向量1的weight
*      @param    weight2   [in]    向量2的weight
*      @return      无
*/    
void VectorMerge(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2, vector<IdWeight> &vecResult, float weight1 = 1.0f, float weight2 = 1.0f);

/**
*      对两个紧缩IdWeight向量求交
*      @param    vecptr1    [in]    向量1的首指针
*      @param    vecsize1   [in]    向量1的长度
*      @param    vecptr2    [in]    向量2的首指针
*      @param    vecsize2   [in]    向量2的长度
*      @param    vecResult  [out]   求交的结果
*      @return      两个向量的点积结果
*/    
float VectorJoin(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2, vector<IdWeight> &vecResult);

/**
*      对两个紧缩IdWeight向量求点积
*      @param    vecptr1    [in]    向量1的首指针
*      @param    vecsize1   [in]    向量1的长度
*      @param    vecptr2    [in]    向量2的首指针
*      @param    vecsize2   [in]    向量2的长度
*      @return      两个向量的点积结果
*/    
float DotProduct(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2);

/**
*      对两个紧缩IdWeight向量求并
*      @param    vecptr1    [in]    向量1的首指针
*      @param    vecsize1   [in]    向量1的长度
*      @param    vecptr2    [in]    向量2的首指针
*      @param    vecsize2   [in]    向量2的长度
*      @param    vecResult  [out]   求并后的结果
*      @param    weight1    [in]    向量1的weight
*      @param    weight2    [in]    向量2的weight
*      @return      无
*/    
void VectorMerge(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2, vector<IdWeight> &vecResult, float weight1 = 1.0f, float weight2 = 1.0f);

/**
*      对IdWeight向量进行归一化
*      @param    srcVec    [in,out]    进行归一化的向量
*      @return      无
*/    
void Normalization(vector<IdWeight> &srcVec);


#endif
