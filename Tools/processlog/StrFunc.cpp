#include "StrFunc.h"

namespace adr
{
/**
 * @brief 去除行末的回车
 * @return 成功返回0，失败返回负数
 */
int chomp(char* line)
{
	char* p = line;
	while (*p)
	{
		if (*p == '\r' || *p == '\n')
		{
			*p = 0;
			break;
		}
		p ++;
	}
	return 0;
}

/**
 * @brief 将字符串按指定的分割符进行分割，获得多个子串
 *        line会被修改
 * @param line 划分前的字符串
 * @param split 分割符
 * @param str_vec 分割结果
 * @return 子串的数量，出错返回负数
 */
int Split(char* line, const char* split, vector<char*>& str_vec)
{
	char *p, *q;

	// 消除line尾部的一些特殊字符
	q = line + strlen(line) - 1;
	while ((*q == '\n' || *q == '\r') && q >= line)
	{
		*q = 0;
		q --;
	}
	// 寻找分割符进行分割
	str_vec.clear();
	p = line;
	while (p != NULL)
	{
		q = strstr(p, split);
		if (q == NULL)
		{
			str_vec.push_back(p);
			break;
		}
		str_vec.push_back(p);
		*q = 0;
		p = q + (int)strlen(split);
	}
	return (int)(str_vec.size());
}

/**
 * @brief 将字符串按指定的分割符进行分割，获得多个子串
 *        line不会被修改
 * @param line 划分前的字符串
 * @param split 分割符
 * @param str_vec 分割结果
 * @return 子串的数量，出错返回负数
 */
int Split(char* line, const char* split, vector<string>& str_vec)
{
	char *p, *q;

	// 消除line尾部的一些特殊字符
	q = line + strlen(line) - 1;
	while ((*q == '\n' || *q == '\r') && q >= line)
	{
		*q = 0;
		q --;
	}
	// 寻找分割符进行分割
	str_vec.clear();
	p = line;
	while (p != NULL)
	{
		q = strstr(p, split);
		if (q == NULL)
		{
			str_vec.push_back(p);
			break;
		}
		str_vec.push_back(string(p, 0, q - p));
		p = q + (int)strlen(split);
	}
	return (int)(str_vec.size());
}

/**
 * @brief 从buf中寻找指定的start, end之间对应的文本
 * @param buf 字符串缓冲区
 * @param start 开始模式
 * @param end 开始模式之后出现的第一个end为结束位置
 * @param result 返回结果
 * @return 成功返回end出现的位置，未找到返回NULL
 */
char* FindPattern(char* buf, const char* start, const char* end, string& result)
{
	int len;
	char *p = NULL, *q = NULL;

	len = (int)strlen(start);
	p = strstr(buf, start);
	if (p == NULL)
	{
		return NULL;
	}
	q = strstr(p + len, end);
	if (q == NULL)
	{
		return NULL;
	}
	result = string(p + len, 0, q - p - len);
	return q;
}

/**
 * @brief 获取字符串的hash值
 * @param str 字符串
 * @param table_len hash表长度
 * @param factor Robin算法的因子
 */
int GetStrHashCode(const char* str, int table_len, int factor)
{
	unsigned int hashcode = 0;
	unsigned char *p;

	p = (unsigned char*)str;
	while (*p)
	{
		hashcode = hashcode * factor + (*p);
		p ++;
	}
	return (int)(hashcode % table_len);
}

}

