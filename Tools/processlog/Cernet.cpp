#include <stdio.h>
#include <stdlib.h>
#include "Cernet.h"
#include "StrFunc.h"
#include "Conv.h"

namespace adr
{
// --------------------------------------------------------------- 公有函数接口1
// 构造函数
CCernet::CCernet()
{
	m_ip_count = 0;
}

// 析构函数
CCernet::~CCernet()
{
	Release();
}

// 加载教育网分段
int CCernet::LoadCernetIP(const char* cernet_ip_file)
{
	FILE* fp = NULL;
	t_location loc;
	char buf[256];
	vector<char*> str_vec;
	int count = 0, bit_count;

	fp = fopen(cernet_ip_file, "rb");
	if (!fp)
	{
		fprintf(stderr, "open ip file error! %s\n", cernet_ip_file);
		return -1;
	}
	m_ip_count = 0;
	loc = CERNET_NONE;
	while (fgets(buf, 256, fp) != NULL)
	{
		if (buf[0] == 'r')
		{
			if ((count = Split(buf, "=", str_vec)) != 2)
			{
				fprintf(stderr, "read line error! split count = %d\n", count); 
				return -1;
			}
			loc = ConvStr_to_Loc(str_vec[1]); 
			if (loc == CERNET_NONE)
			{
				fprintf(stderr, "loc parser error! %s\n", str_vec[1]);
				return -1;
			}
		}
		else
		{
			if (m_ip_count >= MAX_IP_COUNT)
			{
				fprintf(stderr, "too many ip count!\n");
				return -1;
			}
			if (Split(buf, "/", str_vec) != 2)
			{
				fprintf(stderr, "read line error! split count = %d\n", count); 
				return -1;
			}
			ConvIP_to_UInt(str_vec[0], m_ip_table[m_ip_count].ip_head);
			bit_count = atoi(str_vec[1]);
			m_ip_table[m_ip_count].ip_mask = 0;
			for (int i = 0; i < bit_count; i ++)
			{
				m_ip_table[m_ip_count].ip_mask |= (1<<(31-i));
			}
//			printf("ip head = %x, ip mask = %x, bit count = %d\n", m_ip_table[m_ip_count].ip_head, m_ip_table[m_ip_count].ip_mask, bit_count);
			m_ip_table[m_ip_count].location = loc;
			if (loc == CERNET_NONE)
			{
				fprintf(stderr, "loc parser error! %s\n", str_vec[1]);
				return -1;
			}
			m_ip_count ++;
		}
	}
	fclose(fp);
	return 0;
}

// 查询是否为教育网IP，并计算IP地址所在的区域
CCernet::t_location CCernet::GetCernetIP(const char* ip)
{
	unsigned int ipval;

//	printf("ip = %s\n", ip);
	if (ConvIP_to_UInt(ip, ipval) < 0)
	{
		return CERNET_NONE;
	}
	for (int i = 0; i < m_ip_count; i ++)
	{
		if ((m_ip_table[i].ip_mask & ipval) == m_ip_table[i].ip_head)
		{
			return m_ip_table[i].location;
		}
	}
	return CERNET_NONE;
}

// 判断该IP是否为教育网IP
bool CCernet::IsCernetIP(const char* ip)
{
	return (GetCernetIP(ip) != CERNET_NONE);
}

// 查询是否为教育网IP，并计算IP地址所在的区域
CCernet::t_location CCernet::GetCernetIP(unsigned int ip)
{
	for (int i = 0; i < m_ip_count; i ++)
	{
		if ((m_ip_table[i].ip_mask & ip) == m_ip_table[i].ip_head)
		{
			return m_ip_table[i].location;
		}
	}
	return CERNET_NONE;
}

// 判断该IP是否为教育网IP
bool CCernet::IsCernetIP(unsigned int ip)
{
	return (GetCernetIP(ip) != CERNET_NONE);
}

// 释放资源
int CCernet::Release()
{
	return 0;
}

// --------------------------------------------------------------- 私有函数接口1

// 将地域的字符串表示转换成t_location
CCernet::t_location CCernet::ConvStr_to_Loc(const char* loc_str)
{
	if (strcmp(loc_str, "BJ") == 0)
	{
		return CERNET_BJ;
	}
	else if (strcmp(loc_str, "XA") == 0)
	{
		return CERNET_XA;
	}
	else if (strcmp(loc_str, "CD") == 0)
	{
		return CERNET_CD;
	}
	else if (strcmp(loc_str, "GZ") == 0)
	{
		return CERNET_GZ;
	}
	else if (strcmp(loc_str, "WH") == 0)
	{
		return CERNET_WH;
	}
	else if (strcmp(loc_str, "NJ") == 0)
	{
		return CERNET_NJ;
	}
	else if (strcmp(loc_str, "SH") == 0)
	{
		return CERNET_SH;
	}
	else if (strcmp(loc_str, "SY") == 0)
	{
		return CERNET_SY;
	}
	else if (strcmp(loc_str, "MISC") == 0)
	{
		return CERNET_MISC;
	}
	else if (strcmp(loc_str, "CIS") == 0)
	{
		return CERNET_CIS;
	}
	else if (strcmp(loc_str, "TTN") == 0)
	{
		return CERNET_TTN;
	}
	else if (strcmp(loc_str, "TEIN2") == 0)
	{
		return CERNET_TEIN2;
	}
	return CERNET_NONE;
}

}

