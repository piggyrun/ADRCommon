#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <vector>
#include <string>
#include <sstream>
using namespace std;

#include "Platform/md5/md5.h"

const uint32_t  MAX_FIELD_NUM = 128;
const uint32_t  MAX_STRING_LENGTH= 1024;

//通过给定目录名和文件名获取完整文件名
string GetFullPath(const char* dir, const char* filename);

md5_long_32 GetMD5(const uint32_t *termidList, uint32_t termlen);

//通过给定字符分割字符串，ignoreEmpty表示是否忽略分割后的空串
void SplitString(const string &src, char delimitor, vector<string> &dest, bool ignoreEmpty = true);

//通过给定字符串分割字符串，ignoreEmpty表示是否忽略分割后的空串
void SplitString(const string &src, const string &delimitor, vector<string> &dest, bool ignoreEmpty = true);

//通过给定字符分割字符串，老方法，不推荐
int SplitString(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size);

//通过给定字符分割字符串，原函数名，老方法，不推荐
int parse_str(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size);
int parse_str(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], uint32_t * size);
int parse_string(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size);


#endif
