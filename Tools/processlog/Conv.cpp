#include <stdio.h>
#include "Conv.h"

namespace adr
{
// ------------------------------------------------- 公有函数
/**
 * @brief 比较两个md5的大小
 * @param id1 md5 1的id
 * @param id2 md5 2的id
 * @return id1 < id2 返回负数
 *         id1 = id2 返回0
 *         id1 > id2 返回正数
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
 * @brief 将一个md5 id赋值给另一个md5 id
 * @param id_left 被赋值的md5 id
 * @param id_righ 赋值后的md5 id
 */
void AsignMd5Id(t_md5_id& id_left, const t_md5_id& id_right)
{
	for (int i = 0; i < MD5_ID_LEN; i ++)
	{
		id_left[i] = id_right[i];
	}
}

/**
 * @brief 将md5转换成md5 id
 * @param md5 md5
 * @param md5_id md5 id
 * @return 成功返回0，失败返回负数
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
 * @brief 将md5 id还原成md5
 * @param md5_id md5 id
 * @param buf 长度不小于33
 * @return 返回md5
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
 * @brief 获取md5 id的hash值
 * @param md5_id 用户id
 * @param hash_table_len hash表的长度
 * @return user id的hash值
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
 * @brief 比较两个用户的大小
 * @param id1 用户1的id
 * @param id2 用户2的id
 * @return id1 < id2 返回负数
 *         id1 = id2 返回0
 *         id1 > id2 返回正数
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
 * @brief 比较两个用户的大小
 * @param id_left 被赋值的用户id
 * @param id_righ 用户的新id
 */
void AsignUserId(t_user_id& id_left, const t_user_id& id_right)
{
	for (int i = 0; i < USER_ID_LEN; i ++)
	{
		id_left[i] = id_right[i];
	}
}

/**
 * @brief 将用户名转换成用户id
 * @param user_name 用户名
 * @param user_id 用户id
 * @return 成功返回0，失败返回负数
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
 * @brief 将用户名转换成用户id
 * @param user_id 用户id
 * @return 返回用户名
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
 * @brief 判断用户ID是否为空
 * @param user_id 用户id
 * @return 如果用户ID为空，返回false
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
 * @brief 获取user id的hash值
 * @param user_id 用户id
 * @param hash_table_len hash表的长度
 * @return user id的hash值
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
 * @brief 将userid转换成一个浮点数
 * @param userid 用户id
 * @return 成功返回0，失败返回负数
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
 * @brief 将IP的字符串表示转换成unsigned int表示
 * @param ip 字符表示的IP地址，"202.38.64.1"
 * @param value 输出变量，uint表示的IP地址
 * @return 0表示成功，失败返回负数
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
 * @brief 将IP的字符串表示转换成unsigned int表示
 * @param value uint表示的IP地址
 * @param ip 输出值，字符表示的IP地址，"202.38.64.1"，调用者分配空间
 * @return 0表示成功，失败返回负数
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
 * @brief 获取ip的hash值
 * @param ip ip的unsigned int表示
 * @param hash_table_len hash表的长度
 * @param factor hash因子
 * @return user id的hash值
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
 * @brief 计算URL的hash码
 * @param url url
 * @return URL的hash码
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
 * @brief 对url进行处理，去掉分页符和参数
 * @param url 处理前的url，并返回处理后的url
 */
char* CheckUrl(char* url)
{
	// 删除参数
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
	// 删除分页的url中的页号 
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

