#ifndef _ADR_USER_H
#define _ADR_USER_H

#include <inttypes.h>
#include <string>
using namespace std;

namespace adr
{
// md5 id
#define MD5_ID_LEN 16 
typedef unsigned char t_md5_id[MD5_ID_LEN];	// �û�id����16���ֽڱ�ʾ

/**
 * @brief �Ƚ�����md5�Ĵ�С
 * @param id1 md5 1��id
 * @param id2 md5 2��id
 * @return id1 < id2 ���ظ���
 *         id1 = id2 ����0
 *         id1 > id2 ��������
 */
int CmpMd5Id(const t_md5_id& id1, const t_md5_id& id2);

/**
 * @brief ��һ��md5 id��ֵ����һ��md5 id
 * @param id_left ����ֵ��md5 id
 * @param id_righ ��ֵ���md5 id
 */
void AsignMd5Id(t_md5_id& id_left, const t_md5_id& id_right);

/**
 * @brief ��md5ת����md5 id
 * @param md5 md5
 * @param md5_id md5 id
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
int ConvMd5_to_Id(const string& md5, t_md5_id& md5_id);

/**
 * @brief ��md5 id��ԭ��md5
 * @param md5_id md5 id
 * @param buf ���Ȳ�С��33
 * @return ����md5
 */
int ConvMd5Id_to_Md5(const t_md5_id& md5_id, char* buf);

/**
 * @brief ��ȡmd5 id��hashֵ
 * @param md5_id �û�id
 * @param hash_table_len hash��ĳ���
 * @return user id��hashֵ
 */
int GetMd5HashCode(const t_md5_id& md5_id, int hash_table_len, int factor = 17);

// user id
/// @brief sogou user_id��ת���࣬sogou user id���ص�Ϊ128λ������16���Ʊ�ʾ
///        ÿ��user id���ַ�������Ϊ32����ΧΪA-F,0-9
#define USER_ID_LEN 16 
typedef unsigned char t_user_id[USER_ID_LEN];	// �û�id����16���ֽڱ�ʾ

/**
 * @brief �Ƚ������û��Ĵ�С
 * @param id1 �û�1��id
 * @param id2 �û�2��id
 * @return id1 < id2 ���ظ���
 *         id1 = id2 ����0
 *         id1 > id2 ��������
 */
int CmpUserId(const t_user_id& id1, const t_user_id& id2);

/**
 * @brief ��һ��user id��ֵ����һ��user id
 * @param id_left ����ֵ��user id
 * @param id_righ ��ֵ���user id
 */
void AsignUserId(t_user_id& id_left, const t_user_id& id_right);

/**
 * @brief ���û���ת�����û�id
 * @param user_name �û���
 * @param user_id �û�id
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
int ConvUserName_to_Id(const string& user_name, t_user_id& user_id);

/**
 * @brief ���û���ת�����û�id
 * @param user_id �û�id
 * @return �����û���
 */
string ConvUserId_to_Name(const t_user_id& user_id);

/**
 * @brief �ж��û�ID�Ƿ�Ϊ��
 * @param user_id �û�id
 * @return ����û�IDΪ�գ�����false
 */
bool IsZero(const t_user_id& user_id);

/**
 * @brief ��ȡuser id��hashֵ
 * @param user_id �û�id
 * @param hash_table_len hash��ĳ���
 * @return user id��hashֵ
 */
int GetUserHashCode(const t_user_id& user_id, int hash_table_len, int factor = 17);
unsigned int GetUserCode(const t_user_id& user_id, int factor = 19);

/**
 * @brief ��useridת����һ��������
 * @param userid �û�id��userid�����ǺϷ���
 * @return �ɹ�����0��ʧ�ܷ��ظ���
 */
float GetUserNameWeight(const char* userid);

/// @brief IP��ַ��ת������
/**
 * @brief ��IP���ַ�����ʾת����unsigned int��ʾ
 * @param ip �ַ���ʾ��IP��ַ��"202.38.64.1"
 * @param value ���������uint��ʾ��IP��ַ
 * @return 0��ʾ�ɹ���ʧ�ܷ��ظ���
 */
int ConvIP_to_UInt(const char *ip, unsigned int& value);

/**
 * @brief ��IP���ַ�����ʾת����unsigned int��ʾ
 * @param value uint��ʾ��IP��ַ
 * @param ip ���ֵ���ַ���ʾ��IP��ַ��"202.38.64.1"�������߷���ռ�
 * @return 0��ʾ�ɹ���ʧ�ܷ��ظ���
 */
int ConvUInt_to_IP(unsigned int value, char* ip);

/**
 * @brief ��ȡip��hashֵ
 * @param ip ip��unsigned int��ʾ
 * @param hash_table_len hash��ĳ���
 * @param factor hash����
 * @return user id��hashֵ
 */
int GetIPHashCode(unsigned int ip, int hash_table_len, int factor = 17);

/// @brief url��64λhashֵ�������ж�����url�Ƿ����
/**
 * @brief ����URL��hash��
 * @param url url
 * @return URL��hash��
 */
uint64_t GetUrlHash(const char* url, int factor = 17);

/**
 * @brief ��url���д���ȥ����ҳ���Ͳ���
 * @param url ����ǰ��url
 * @return ���ش�����url
 */
char* CheckUrl(char* url);

}
#endif

