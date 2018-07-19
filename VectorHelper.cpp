#include "VectorHelper.h"
#include <math.h>

bool IdWeightSortId(const IdWeight &item1, const IdWeight &item2)
{
    return item1.id < item2.id;
}

bool IdWeightSortWeight(const IdWeight &item1, const IdWeight &item2)
{
    return item1.weight > item2.weight;
}

void GetIdWeightArray(vector<IdWeight> &vecInput, uint32_t *idlist, unsigned char *weightList, uint32_t &length)
{
    length = min(length, uint32_t(vecInput.size()));
    if (length < vecInput.size())
    {
        partial_sort(vecInput.begin(), vecInput.begin() + length, vecInput.end(), IdWeightSortWeight);
        sort(vecInput.begin(), vecInput.begin() + length, IdWeightSortId);
    }

    for (uint32_t i = 0; i < length; i++)
    {
        idlist[i] = vecInput[i].id;
        weightList[i] = (unsigned char)(vecInput[i].weight * 256.0f);
    }
}

void BuildVector(const uint32_t *idlist, const unsigned char *weightList, uint32_t length, vector<IdWeight> &vecResult)
{
    vecResult.clear();
    vecResult.resize(length);
    for (uint32_t i = 0; i < length; i++)
    {            
        vecResult[i].id = idlist[i];
        vecResult[i].weight = float(weightList[i]) / 256.0f;
    }
}

float DotProduct(const uint32_t *idlist1, const unsigned char *weightList1, uint32_t length1, const uint32_t *idlist2, const unsigned char *weightList2, uint32_t length2)
{
    //float dotProduct = 0.0f;

    size_t index1 = 0, index2 = 0; //索引指针

	uint32_t tempResult = 0;

    while (index1 < length1 && index2 < length2) //两个同现词数据都未比较完
    {
        if (idlist1[index1] < idlist2[index2])  //第二个向量当前词数值较大
        {
            index1++;
        } 
        else if (idlist1[index1] > idlist2[index2]) //第一个向量当前词数值较大
        {
            index2++;
        }
        else //两个向量当前词数值一样
        {
            //float product = float(weightList1[index1] * weightList2[index2]) / 65536.0f; //求两者weight乘积，用作新weight及点积
			uint32_t product = uint32_t(weightList1[index1] * weightList2[index2]);
            tempResult += product; //计算点积

            index1++;
            index2++;
        }
    }

    return float(tempResult) / 65536.0f;

}

float DotProduct(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2)
{
    float dotProduct = 0.0f;

    //获得输入词的同向词数量
    size_t size1 = vec1.size();
    size_t size2 = vec2.size();

    size_t index1 = 0, index2 = 0; //索引指针

    //设置数组型指针指向两个输入此的同现词向量
    const IdWeight *ptrVec1 = &vec1[0];
    const IdWeight *ptrVec2 = &vec2[0];

    while (index1 < size1 && index2 < size2) //两个同现词数据都未比较完
    {
        if (ptrVec1[index1].id < ptrVec2[index2].id)  //第二个向量当前词数值较大
        {
            index1++;
        } 
        else if (ptrVec1[index1].id > ptrVec2[index2].id) //第一个向量当前词数值较大
        {
            index2++;
        }
        else //两个向量当前词数值一样
        {
            float product = ptrVec1[index1].weight * ptrVec2[index2].weight; //求两者weight乘积，用作新weight及点积
            dotProduct += product; //计算点积

            index1++;
            index2++;
        }
    }

    return dotProduct;
}

float VectorJoin(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2, vector<IdWeight> &vecResult)
{
    float dotProduct = 0.0f;
    vecResult.clear();

    //获得输入词的同向词数量
    size_t size1 = vec1.size();
    size_t size2 = vec2.size();

    size_t index1 = 0, index2 = 0; //索引指针

    //设置数组型指针指向两个输入此的同现词向量
    const IdWeight *ptrVec1 = &vec1[0];
    const IdWeight *ptrVec2 = &vec2[0];

    while (index1 < size1 && index2 < size2) //两个同现词数据都未比较完
    {
        if (ptrVec1[index1].id < ptrVec2[index2].id)  //第二个向量当前词数值较大
        {
            index1++;
        } 
        else if (ptrVec1[index1].id > ptrVec2[index2].id) //第一个向量当前词数值较大
        {
            index2++;
        }
        else //两个向量当前词数值一样
        {
            float product = ptrVec1[index1].weight * ptrVec2[index2].weight; //求两者weight乘积，用作新weight及点积
            dotProduct += product; //计算点积

            IdWeight newIdWeight;
            newIdWeight.id = vec1[index1].id;
            newIdWeight.weight = vec1[index1].weight * vec2[index2].weight;
            vecResult.push_back(newIdWeight);

            index1++;
            index2++;
        }
    }

    return dotProduct;

}
void VectorMerge(const vector<IdWeight> &vec1, const vector<IdWeight> &vec2, vector<IdWeight> &vecResult, float weight1, float weight2)
{
    vecResult.clear();
    
    size_t index1 = 0, index2 = 0; //索引指针
    size_t size1 = vec1.size();
    size_t size2 = vec2.size();

    while (index1 < size1 && index2 < size2) //两个同现词数据都未比较完
    {
        if (vec1[index1].id < vec2[index2].id)  //第二个向量当前词数值较大
        {
            IdWeight newIdWeight;
            newIdWeight.id = vec1[index1].id;
            newIdWeight.weight = vec1[index1].weight * weight1;
            vecResult.push_back(newIdWeight);

            index1++;
            
        } 
        else if (vec1[index1].id > vec2[index2].id) //第一个向量当前词数值较大
        {
            IdWeight newIdWeight;
            newIdWeight.id = vec2[index2].id;
            newIdWeight.weight = vec2[index2].weight * weight2;
            vecResult.push_back(newIdWeight);

            index2++;
        }
        else //两个向量当前词数值一样
        {
            IdWeight newIdWeight;
            newIdWeight.id = vec1[index1].id;
            newIdWeight.weight = vec1[index1].weight * weight1 + vec2[index2].weight * weight2;
            vecResult.push_back(newIdWeight);

            index1++;
            index2++;
        }

        //printf("%d\t%d\t%d\t%d\n", index1, index2, vec1[index1].id, vec2[index2].id);
    }

    for (size_t i = index1; i < size1; i++)
    {
        IdWeight newIdWeight;
        newIdWeight.id = vec1[i].id;
        newIdWeight.weight = vec1[i].weight * weight1;
        vecResult.push_back(newIdWeight);
    }

    for (size_t i = index2; i < size2; i++)
    {
        IdWeight newIdWeight;
        newIdWeight.id = vec2[i].id;
        newIdWeight.weight = vec2[i].weight * weight2;
        vecResult.push_back(newIdWeight);
    }
}


float VectorJoin(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2, vector<IdWeight> &vecResult)
{
    float dotProduct = 0.0f;
    vecResult.clear();


    size_t index1 = 0, index2 = 0; //索引指针

    while (index1 < vecsize1 && index2 < vecsize2) //两个同现词数据都未比较完
    {
        if (vecptr1[index1].id < vecptr2[index2].id)  //第二个向量当前词数值较大
        {
            index1++;
        } 
        else if (vecptr1[index1].id > vecptr2[index2].id) //第一个向量当前词数值较大
        {
            index2++;
        }
        else //两个向量当前词数值一样
        {
            float product = float(vecptr1[index1].weight * vecptr2[index2].weight) / 65536.0f; //求两者weight乘积，用作新weight及点积

            IdWeight newIdWeight;
            newIdWeight.id = vecptr1[index1].id;
            newIdWeight.weight = product;
            vecResult.push_back(newIdWeight);

            dotProduct += product; //计算点积

            index1++;
            index2++;
        }
    }

    return dotProduct;
}

float DotProduct(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2)
{
    float dotProduct = 0.0f;

    size_t index1 = 0, index2 = 0; //索引指针

    while (index1 < vecsize1 && index2 < vecsize2) //两个同现词数据都未比较完
    {
        if (vecptr1[index1].id < vecptr2[index2].id)  //第二个向量当前词数值较大
        {
            index1++;
        } 
        else if (vecptr1[index1].id > vecptr2[index2].id) //第一个向量当前词数值较大
        {
            index2++;
        }
        else //两个向量当前词数值一样
        {
            float product = float(vecptr1[index1].weight * vecptr2[index2].weight) / 65536.0f; //求两者weight乘积，用作新weight及点积

            dotProduct += product; //计算点积

            index1++;
            index2++;
        }
    }

    return dotProduct;
}


void VectorMerge(const IdWeightS *vecptr1, uint32_t vecsize1, const IdWeightS *vecptr2, uint32_t vecsize2, vector<IdWeight> &vecResult, float weight1, float weight2)
{
    vecResult.clear();

    size_t index1 = 0, index2 = 0; //索引指针

    while (index1 < vecsize1 && index2 < vecsize2) //两个同现词数据都未比较完
    {
        if (vecptr1[index1].id < vecptr2[index2].id)  //第二个向量当前词数值较大
        {
            IdWeight newIdWeight;
            newIdWeight.id = vecptr1[index1].id;
            newIdWeight.weight = float(vecptr1[index1].weight * weight1) / 256.0f;
            vecResult.push_back(newIdWeight);

            index1++;
        } 
        else if (vecptr1[index1].id > vecptr2[index2].id) //第一个向量当前词数值较大
        {
            IdWeight newIdWeight;
            newIdWeight.id = vecptr2[index2].id;
            newIdWeight.weight = float(vecptr2[index2].weight * weight2) / 256.0f;
            vecResult.push_back(newIdWeight);

            index2++;
        }
        else //两个向量当前词数值一样
        {
            IdWeight newIdWeight;
            newIdWeight.id = vecptr1[index1].id;
            newIdWeight.weight = float(vecptr1[index1].weight * weight1 + vecptr2[index2].weight * weight2) / 256.0f;;
            vecResult.push_back(newIdWeight);

            index1++;
            index2++;
        }
    }
}


void Normalization(vector<IdWeight> &srcVec)
{
    float length = 0.0f;

    //求向量长度
    for (size_t i = 0; i < srcVec.size(); i++)
    {
        length += srcVec[i].weight * srcVec[i].weight;
    }
    length = sqrt(length);

    //对向量元素进行归一化
    for (size_t i = 0; i < srcVec.size(); i++)
    {
        srcVec[i].weight = srcVec[i].weight / length;
    }
}
