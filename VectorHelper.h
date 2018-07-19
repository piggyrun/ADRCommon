/**
 * @Author  Guo Ang 
 * @Date  2010-4-7
 * @Brief ��������������
 */

#ifndef VECTOR_HELPER_H
#define VECTOR_HELPER_H

#include <vector>
#include <string>
#include <sstream>
using namespace std;

//#include "CommonTools.h"

//����IdWeight
struct IdWeight
{
    uint32_t id;
    float weight;
};

//�����ܼ���IdWeight
struct IdWeightS
{
    unsigned id:24;     
    unsigned weight:8;  
};

//����id��IdWeight��������ıȽϺ���
bool IdWeightSortId(const IdWeight &item1, const IdWeight &item2); 

//����weight��IdWeight��������ıȽϺ���
bool IdWeightSortWeight(const IdWeight &item1, const IdWeight &item2); 


void GetIdWeightArray(vector<IdWeight> &vecInput, uint32_t *idlist, unsigned char *weightList, uint32_t &length);


float DotProduct(const uint32_t *idlist1, const unsigned char *weightList1, uint32_t length1, const uint32_t *idlist2, const unsigned char *weightList2, uint32_t length2);

/**
*      ͨ�����齨��IdWeight����
*      @param    idlist     [in]    id����
*      @param    weightList [in]    weight����
*      @param    length     [in]    ���鳤��
*      @param    vecResult  [out]   �������
*      @return   ��
*/    
void BuildVector(const uint32_t *idlist, const unsigned char *weightList, uint32_t length, vector<IdWeight> &vecResult);

/**
*      ��ȡ����IdWeight�����ĵ��
*      @param    vec1     [in]    ����1
*      @param    vec2     [in]    ����2
*      @return      ������
*/    
float DotProduct(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2);


/**
*      ��ȡ����IdWeight�������󽻺͵��
*      @param    vec1      [in]    ����1
*      @param    vec2      [in]    ����2
*      @param    vecResult [out]   �󽻺�Ľ��
*      @return      ������
*/    
float VectorJoin(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2, vector<IdWeight> &vecResult);

/**
*      ������IdWeight�������кϲ�
*      @param    vec1      [in]    ����1
*      @param    vec2      [in]    ����2
*      @param    vecResult [out]   �󲢺�Ľ��
*      @param    weight1   [in]    ����1��weight
*      @param    weight2   [in]    ����2��weight
*      @return      ��
*/    
void VectorMerge(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2, vector<IdWeight> &vecResult, float weight1 = 1.0f, float weight2 = 1.0f);

/**
*      ����������IdWeight������
*      @param    vecptr1    [in]    ����1����ָ��
*      @param    vecsize1   [in]    ����1�ĳ���
*      @param    vecptr2    [in]    ����2����ָ��
*      @param    vecsize2   [in]    ����2�ĳ���
*      @param    vecResult  [out]   �󽻵Ľ��
*      @return      ���������ĵ�����
*/    
float VectorJoin(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2, vector<IdWeight> &vecResult);

/**
*      ����������IdWeight��������
*      @param    vecptr1    [in]    ����1����ָ��
*      @param    vecsize1   [in]    ����1�ĳ���
*      @param    vecptr2    [in]    ����2����ָ��
*      @param    vecsize2   [in]    ����2�ĳ���
*      @return      ���������ĵ�����
*/    
float DotProduct(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2);

/**
*      ����������IdWeight������
*      @param    vecptr1    [in]    ����1����ָ��
*      @param    vecsize1   [in]    ����1�ĳ���
*      @param    vecptr2    [in]    ����2����ָ��
*      @param    vecsize2   [in]    ����2�ĳ���
*      @param    vecResult  [out]   �󲢺�Ľ��
*      @param    weight1    [in]    ����1��weight
*      @param    weight2    [in]    ����2��weight
*      @return      ��
*/    
void VectorMerge(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2, vector<IdWeight> &vecResult, float weight1 = 1.0f, float weight2 = 1.0f);

/**
*      ��IdWeight�������й�һ��
*      @param    srcVec    [in,out]    ���й�һ��������
*      @return      ��
*/    
void Normalization(vector<IdWeight> &srcVec);


#endif
