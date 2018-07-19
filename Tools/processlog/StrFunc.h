#ifndef _ADR_STRFUNC_H
#define _ADR_STRFUNC_H

#include <vector>
#include <string>
using namespace std;

namespace adr
{

/**
 * @brief ȥ����ĩ�Ļس�
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
int chomp(char* line);

/**
 * @brief ���ַ�����ָ���ķָ�����зָ��ö���Ӵ�
 *        line�ᱻ�޸�
 * @param line ����ǰ���ַ���
 * @param split �ָ��
 * @param str_vec �ָ���
 * @return �Ӵ��������������ظ���
 */
int Split(char* line, const char* split, vector<char*>& str_vec);

/**
 * @brief ���ַ�����ָ���ķָ�����зָ��ö���Ӵ�
 *        line���ᱻ�޸�
 * @param line ����ǰ���ַ���
 * @param split �ָ��
 * @param str_vec �ָ���
 * @return �Ӵ��������������ظ���
 */
int Split(char* line, const char* split, vector<string>& str_vec);

/**
 * @brief ��buf��Ѱ��ָ����start, end֮���Ӧ���ı�
 * @param buf �ַ���������
 * @param start ��ʼģʽ
 * @param end ��ʼģʽ֮����ֵĵ�һ��endΪ����λ��
 * @param result ���ؽ��
 * @return �ɹ�����end���ֵ�λ�ã�δ�ҵ�����NULL
 */
char* FindPattern(char* buf, const char* start, const char* end, string& result);

/**
 * @brief ��ȡ�ַ�����hashֵ
 * @param str �ַ���
 * @param table_len hash����
 * @param factor Robin�㷨������
 */
int GetStrHashCode(const char* str, int table_len, int factor = 17);

}

#endif

