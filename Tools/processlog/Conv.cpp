#include <stdio.h>
#include "Conv.h"

namespace adr
{
// ------------------------------------------------- ���к���
/**
 * @brief �Ƚ�����md5�Ĵ�С
 * @param id1 md5 1��id
 * @param id2 md5 2��id
 * @return id1 < id2 ���ظ���
 *         id1 = id2 ����0
 *         id1 > id2 ��������
 */
int CmpMd5Id(const t_md5_id& id1, const t_md5_id& id2)
{
	int i = 0;

	while (i < MD5_ID_LEN && id1[i] == id2[i])
	{
		i ++;
	}
	return (i < MD5_ID_LEN) ? (int)(id1[i] - id2[i]) : 0;
}

/**
 * @brief ��һ��md5 id��ֵ����һ��md5 id
 * @param id_left ����ֵ��md5 id
 * @param id_righ ��ֵ���md5 id
 */
void AsignMd5Id(t_md5_id& id_left, const t_md5_id& id_right)
{
	for (int i = 0; i < MD5_ID_LEN; i ++)
	{
		id_left[i] = id_right[i];
	}
}

/**
 * @brief ��md5ת����md5 id
 * @param md5 md5
 * @param md5_id md5 id
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
int ConvMd5_to_Id(const string& md5, t_md5_id& md5_id)
{
	unsigned char low, high;
	unsigned char* p = NULL;
	p = (unsigned char*)md5.c_str();

	if (md5.length() != 32)
	{
		return -1;
	}
	for (int i = 0; i < MD5_ID_LEN; i ++)
	{
		low = *p;
		high = *(p + 1);
		if (low <= 'f' && low >= 'a')
		{
			low = low - 'a' + 10;
		}
		else if (low <= '9' && low >= '0')
		{
			low = low - '0';
		}
		else
		{
			return -2;
		}
		if (high <= 'f' && high >= 'a')
		{
			high = high - 'a' + 10;
		}
		else if (high <= '9' && high >= '0')
		{
			high = high - '0';
		}
		else
		{
			return -2;
		}
		md5_id[i] = low * 16 + high;
		p += 2;
	}
	return 0;
}

/**
 * @brief ��md5 id��ԭ��md5
 * @param md5_id md5 id
 * @param buf ���Ȳ�С��33
 * @return ����md5
 */
int ConvMd5Id_to_Md5(const t_md5_id& md5_id, char* buf)
{
	unsigned char low, high;
	for (int i = 0; i < MD5_ID_LEN; i ++)
	{
		low = md5_id[i] >> 4;
		high = (unsigned char)(md5_id[i] & 0xF);
		if (low < 10)
		{
			low = low + '0';
		}
		else
		{
			low = low - 10 + 'a';
		}
		if (high < 10)
		{
			high = high + '0';
		}
		else
		{
			high = high - 10 + 'a';
		}
		buf[2 * i] = (char)low;
		buf[2 * i + 1] = (char)high;
	}
	buf[2 * MD5_ID_LEN] = 0;
	return 0;
}

/**
 * @brief ��ȡmd5 id��hashֵ
 * @param md5_id �û�id
 * @param hash_table_len hash��ĳ���
 * @return user id��hashֵ
 */
int GetMd5HashCode(const t_md5_id& md5_id, int hash_table_len, int factor)
{
    unsigned int hashcode = 0; 
	for (int i = 0; i < MD5_ID_LEN; i ++)
	{    
		hashcode = (hashcode * factor) + md5_id[i];
	}    
	return (int)(hashcode % hash_table_len);
}

/**
 * @brief �Ƚ������û��Ĵ�С
 * @param id1 �û�1��id
 * @param id2 �û�2��id
 * @return id1 < id2 ���ظ���
 *         id1 = id2 ����0
 *         id1 > id2 ��������
 */
int CmpUserId(const t_user_id& id1, const t_user_id& id2)
{
	int i = 0;

	while (i < USER_ID_LEN && id1[i] == id2[i])
	{
		i ++;
	}
	return (i < USER_ID_LEN) ? (int)(id1[i] - id2[i]) : 0;
}

/**
 * @brief �Ƚ������û��Ĵ�С
 * @param id_left ����ֵ���û�id
 * @param id_righ �û�����id
 */
void AsignUserId(t_user_id& id_left, const t_user_id& id_right)
{
	for (int i = 0; i < USER_ID_LEN; i ++)
	{
		id_left[i] = id_right[i];
	}
}

/**
 * @brief ���û���ת�����û�id
 * @param user_name �û���
 * @param user_id �û�id
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
int ConvUserName_to_Id(const string& user_name, t_user_id& user_id)
{
	unsigned char low, high;
	unsigned char* p = NULL;
	p = (unsigned char*)user_name.c_str();

	if (user_name.length() != 32)
	{
//		fprintf(stderr, "Warning: UserName = %s\n", user_name.c_str());
		return -1;
	}
	if (user_name == "00000000000000000000000000000000")
	{
//		fprintf(stderr, "Warning: UserName = %s\n", user_name.c_str());
		return -2;
	}
	for (int i = 0; i < USER_ID_LEN; i ++)
	{
		low = *p;
		high = *(p + 1);
		if (low <= 'F' && low >= 'A')
		{
			low = low - 'A' + 10;
		}
		else if (low <= '9' && low >= '0')
		{
			low = low - '0';
		}
		else
		{
//			fprintf(stderr, "Warning: UserName = %s\n", user_name.c_str());
			return -2;
		}
		if (high <= 'F' && high >= 'A')
		{
			high = high - 'A' + 10;
		}
		else if (high <= '9' && high >= '0')
		{
			high = high - '0';
		}
		else
		{
//			fprintf(stderr, "Warning: UserName = %s\n", user_name.c_str());
			return -3;
		}
		user_id[i] = low * 16 + high;
		p += 2;
	}
	return 0;
}

/**
 * @brief ���û���ת�����û�id
 * @param user_id �û�id
 * @return �����û���
 */
string ConvUserId_to_Name(const t_user_id& user_id)
{
	char buf[2 * USER_ID_LEN + 1];
	unsigned char low, high;
	for (int i = 0; i < USER_ID_LEN; i ++)
	{
		low = user_id[i] >> 4;
		high = (unsigned char)(user_id[i] & 0xF);
		if (low < 10)
		{
			low = low + '0';
		}
		else
		{
			low = low - 10 + 'A';
		}
		if (high < 10)
		{
			high = high + '0';
		}
		else
		{
			high = high - 10 + 'A';
		}
		buf[2 * i] = (char)low;
		buf[2 * i + 1] = (char)high;
	}
	buf[2 * USER_ID_LEN] = 0;
	return string(buf, 0, 2 * USER_ID_LEN);
}

/**
 * @brief �ж��û�ID�Ƿ�Ϊ��
 * @param user_id �û�id
 * @return ����û�IDΪ�գ�����false
 */
bool IsZero(const t_user_id& user_id)
{
	for (int i = 0; i < USER_ID_LEN; i ++)
	{
		if (user_id[i] != 0)
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief ��ȡuser id��hashֵ
 * @param user_id �û�id
 * @param hash_table_len hash��ĳ���
 * @return user id��hashֵ
 */
int GetUserHashCode(const t_user_id& user_id, int hash_table_len, int factor)
{
    unsigned int hashcode = 0; 
	for (int i = 0; i < USER_ID_LEN; i ++)
	{    
		hashcode = (hashcode * factor) + user_id[i];
	}    
	return (int)(hashcode % hash_table_len);
}
unsigned int GetUserCode(const t_user_id& user_id, int factor)
{
    unsigned int hashcode = 0; 
	for (int i = 0; i < USER_ID_LEN; i ++)
	{    
		hashcode = (hashcode * factor) + user_id[i];
	}    
	return hashcode;
}

/**
 * @brief ��useridת����һ��������
 * @param userid �û�id
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
float GetUserNameWeight(const char* userid)
{
	unsigned int value = 0;
	unsigned char* p = NULL;

	p = (unsigned char*)userid;
	while (*p)
	{   
		value = value * 17 + (*p);
		p ++; 
	}   
	return (value % 10000) / 10000.0;
}

/**
 * @brief ��IP���ַ�����ʾת����unsigned int��ʾ
 * @param ip �ַ���ʾ��IP��ַ��"202.38.64.1"
 * @param value ���������uint��ʾ��IP��ַ
 * @return 0��ʾ�ɹ���ʧ�ܷ��ظ���
 */
int ConvIP_to_UInt(const char *ip, unsigned int& value)
{
	unsigned int a,b,c,d;
	value = 0;
	if (sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) == 4)
	{
		if (a < 256 && b < 256 && c < 256 && d < 256)
		{
			value = (a<<24) + (b<<16) + (c<<8) + d;
			return 0;
		}
	}
	return -1;
}

/**
 * @brief ��IP���ַ�����ʾת����unsigned int��ʾ
 * @param value uint��ʾ��IP��ַ
 * @param ip ���ֵ���ַ���ʾ��IP��ַ��"202.38.64.1"�������߷���ռ�
 * @return 0��ʾ�ɹ���ʧ�ܷ��ظ���
 */
int ConvUInt_to_IP(unsigned int value, char* ip)
{
	unsigned int a = 0, b = 0, c = 0, d = 0;

	a = (value >> 24);
	b = ((value >> 16) & 0xFF);
	c = ((value >> 8) & 0xFF);
	d = (value & 0xFF);

	sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
	return 0;
}

/**
 * @brief ��ȡip��hashֵ
 * @param ip ip��unsigned int��ʾ
 * @param hash_table_len hash��ĳ���
 * @param factor hash����
 * @return user id��hashֵ
 */
int GetIPHashCode(unsigned int ip, int hash_table_len, int factor)
{
	unsigned int hashcode = 0;
	unsigned int a = 0, b = 0, c = 0, d = 0;
	a = (ip >> 24);
	b = ((ip >> 16) & 0xFF);
	c = ((ip >> 8) & 0xFF);
	d = (ip & 0xFF);
	
	hashcode = ((a * 17 + b) * 17 + c) * 17 + d;
	return (int)(hashcode % hash_table_len);
}

/**
 * @brief ����URL��hash��
 * @param url url
 * @return URL��hash��
 */
uint64_t GetUrlHash(const char* url, int factor)
{
	uint64_t hashcode = 0;
	unsigned char* p = (unsigned char*)url;
	while (*p && *p != '?')
	{
		hashcode = hashcode * factor + (*p);
		p ++;
	}
	return hashcode;
}

/**
 * @brief ��url���д���ȥ����ҳ���Ͳ���
 * @param url ����ǰ��url�������ش�����url
 */
char* CheckUrl(char* url)
{
	// ɾ������
	char* p = url;
	while (*p)
	{
		if (*p == '?')
		{
			*p = 0;
			break;
		}
		p ++;
	}
	// ɾ����ҳ��url�е�ҳ�� 
	int len, point_pos, line_pos;
	len = strlen(url) - 1;
	point_pos = len;
	while (len > 0 && url[len] != '.')
	{
		len --;
		point_pos = len;
	}
	line_pos = len;
	while (len > 0 && url[len] != '_' && url[len] != '/')
	{
		len --;
		line_pos = len;
	}
	if (url[len] == '_')
	{
		strcpy(url + line_pos, url + point_pos);
	}
	return url;
}

}

