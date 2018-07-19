#ifndef _ADR_STRFUNC_H
#define _ADR_STRFUNC_H

#include <vector>
#include <string>
using namespace std;

namespace adr
{

/**
 * @brief 去除行末的回车
 * @return 成功返回0，失败返回负数
 */
int chomp(char* line);

/**
 * @brief 将字符串按指定的分割符进行分割，获得多个子串
 *        line会被修改
 * @param line 划分前的字符串
 * @param split 分割符
 * @param str_vec 分割结果
 * @return 子串的数量，出错返回负数
 */
int Split(char* line, const char* split, vector<char*>& str_vec);

/**
 * @brief 将字符串按指定的分割符进行分割，获得多个子串
 *        line不会被修改
 * @param line 划分前的字符串
 * @param split 分割符
 * @param str_vec 分割结果
 * @return 子串的数量，出错返回负数
 */
int Split(char* line, const char* split, vector<string>& str_vec);

/**
 * @brief 从buf中寻找指定的start, end之间对应的文本
 * @param buf 字符串缓冲区
 * @param start 开始模式
 * @param end 开始模式之后出现的第一个end为结束位置
 * @param result 返回结果
 * @return 成功返回end出现的位置，未找到返回NULL
 */
char* FindPattern(char* buf, const char* start, const char* end, string& result);

/**
 * @brief 获取字符串的hash值
 * @param str 字符串
 * @param table_len hash表长度
 * @param factor Robin算法的因子
 */
int GetStrHashCode(const char* str, int table_len, int factor = 17);

}

#endif

