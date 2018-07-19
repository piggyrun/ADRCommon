#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H

#include <vector>
#include <string>
#include <sstream>
using namespace std;

#include "Platform/md5/md5.h"

const uint32_t  MAX_FIELD_NUM = 128;
const uint32_t  MAX_STRING_LENGTH= 1024;

//ͨ������Ŀ¼�����ļ�����ȡ�����ļ���
string GetFullPath(const char* dir, const char* filename);

md5_long_32 GetMD5(const uint32_t *termidList, uint32_t termlen);

//ͨ�������ַ��ָ��ַ�����ignoreEmpty��ʾ�Ƿ���Էָ��Ŀմ�
void SplitString(const string &src, char delimitor, vector<string> &dest, bool ignoreEmpty = true);

//ͨ�������ַ����ָ��ַ�����ignoreEmpty��ʾ�Ƿ���Էָ��Ŀմ�
void SplitString(const string &src, const string &delimitor, vector<string> &dest, bool ignoreEmpty = true);

//ͨ�������ַ��ָ��ַ������Ϸ��������Ƽ�
int SplitString(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size);

//ͨ�������ַ��ָ��ַ�����ԭ���������Ϸ��������Ƽ�
int parse_str(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size);
int parse_str(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], uint32_t * size);
int parse_string(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size);


#endif
