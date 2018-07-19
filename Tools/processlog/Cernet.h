#ifndef _ADR_CERNET_H
#define _ADR_CERNET_H

namespace adr
{
class CCernet
{
public:     // 公有数据结构

	// 教育网地域
	typedef enum {
		CERNET_NONE = 0,    // 非教育网IP
		CERNET_BJ = 1,
		CERNET_XA = 2,
		CERNET_CD = 3,
		CERNET_GZ = 4,
		CERNET_WH = 5,
		CERNET_NJ = 6,
		CERNET_SH = 7,
		CERNET_SY = 8,
		CERNET_MISC = 9,
		CERNET_CIS = 10,
		CERNET_TTN = 11,
		CERNET_TEIN2 = 12
	} t_location;	

public:     // 公有函数接口

	// 构造函数
	CCernet();

	// 析构函数
	~CCernet();

	// 加载教育网分段
	int LoadCernetIP(const char* cernet_ip_file);

	// 查询是否为教育网IP，并计算IP地址所在的区域
	t_location GetCernetIP(const char* ip);

	// 判断该IP是否为教育网IP
	bool IsCernetIP(const char* ip);

	// 查询是否为教育网IP，并计算IP地址所在的区域
	t_location GetCernetIP(unsigned int ip);

	// 判断该IP是否为教育网IP
	bool IsCernetIP(unsigned int ip);

	// 释放资源
	int Release();

private:     // 私有数据结构
	
	// 常数定义
	enum {
		MAX_IP_COUNT = 1024      // IP数量的最大值
	};

	// ip节点
	struct t_ip_node
	{
		unsigned int ip_head;
		unsigned int ip_mask;
		t_location location;
	};

private:    // 私有函数接口

	// 将地域的字符串表示转换成t_location
	t_location ConvStr_to_Loc(const char* loc_str);

private:    // 私有数据
	
	t_ip_node m_ip_table[MAX_IP_COUNT];   // ip表
	int m_ip_count;   // ip数量

};
}

#endif

