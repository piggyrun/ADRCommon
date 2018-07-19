#ifndef _ADR_CERNET_H
#define _ADR_CERNET_H

namespace adr
{
class CCernet
{
public:     // �������ݽṹ

	// ����������
	typedef enum {
		CERNET_NONE = 0,    // �ǽ�����IP
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

public:     // ���к����ӿ�

	// ���캯��
	CCernet();

	// ��������
	~CCernet();

	// ���ؽ������ֶ�
	int LoadCernetIP(const char* cernet_ip_file);

	// ��ѯ�Ƿ�Ϊ������IP��������IP��ַ���ڵ�����
	t_location GetCernetIP(const char* ip);

	// �жϸ�IP�Ƿ�Ϊ������IP
	bool IsCernetIP(const char* ip);

	// ��ѯ�Ƿ�Ϊ������IP��������IP��ַ���ڵ�����
	t_location GetCernetIP(unsigned int ip);

	// �жϸ�IP�Ƿ�Ϊ������IP
	bool IsCernetIP(unsigned int ip);

	// �ͷ���Դ
	int Release();

private:     // ˽�����ݽṹ
	
	// ��������
	enum {
		MAX_IP_COUNT = 1024      // IP���������ֵ
	};

	// ip�ڵ�
	struct t_ip_node
	{
		unsigned int ip_head;
		unsigned int ip_mask;
		t_location location;
	};

private:    // ˽�к����ӿ�

	// ��������ַ�����ʾת����t_location
	t_location ConvStr_to_Loc(const char* loc_str);

private:    // ˽������
	
	t_ip_node m_ip_table[MAX_IP_COUNT];   // ip��
	int m_ip_count;   // ip����

};
}

#endif

