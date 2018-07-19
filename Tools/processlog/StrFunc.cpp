#include "StrFunc.h"

namespace adr
{
/**
 * @brief ȥ����ĩ�Ļس�
 * @return �ɹ�����0��ʧ�ܷ��ظ���
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
 * @brief ���ַ�����ָ���ķָ�����зָ��ö���Ӵ�
 *        line�ᱻ�޸�
 * @param line ����ǰ���ַ���
 * @param split �ָ��
 * @param str_vec �ָ���
 * @return �Ӵ��������������ظ���
 */
int Split(char* line, const char* split, vector<char*>& str_vec)
{
	char *p, *q;

	// ����lineβ����һЩ�����ַ�
	q = line + strlen(line) - 1;
	while ((*q == '\n' || *q == '\r') && q >= line)
	{
		*q = 0;
		q --;
	}
	// Ѱ�ҷָ�����зָ�
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
 * @brief ���ַ�����ָ���ķָ�����зָ��ö���Ӵ�
 *        line���ᱻ�޸�
 * @param line ����ǰ���ַ���
 * @param split �ָ��
 * @param str_vec �ָ���
 * @return �Ӵ��������������ظ���
 */
int Split(char* line, const char* split, vector<string>& str_vec)
{
	char *p, *q;

	// ����lineβ����һЩ�����ַ�
	q = line + strlen(line) - 1;
	while ((*q == '\n' || *q == '\r') && q >= line)
	{
		*q = 0;
		q --;
	}
	// Ѱ�ҷָ�����зָ�
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
 * @brief ��buf��Ѱ��ָ����start, end֮���Ӧ���ı�
 * @param buf �ַ���������
 * @param start ��ʼģʽ
 * @param end ��ʼģʽ֮����ֵĵ�һ��endΪ����λ��
 * @param result ���ؽ��
 * @return �ɹ�����end���ֵ�λ�ã�δ�ҵ�����NULL
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
 * @brief ��ȡ�ַ�����hashֵ
 * @param str �ַ���
 * @param table_len hash����
 * @param factor Robin�㷨������
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

