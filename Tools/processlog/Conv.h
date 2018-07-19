#ifndef _ADR_USER_H
#define _ADR_USER_H

#include <inttypes.h>
#include <string>
using namespace std;

namespace adr
{
// md5 id
#define MD5_ID_LEN 16 
typedef unsigned char t_md5_id[MD5_ID_LEN];	// 用户id，用16各字节表示

/**
 * @brief 比较两个md5的大小
 * @param id1 md5 1的id
 * @param id2 md5 2的id
 * @return id1 < id2 返回负数
 *         id1 = id2 返回0
 *         id1 > id2 返回正数
 */
int CmpMd5Id(const t_md5_id& id1, const t_md5_id& id2);

/**
 * @brief 将一个md5 id赋值给另一个md5 id
 * @param id_left 被赋值的md5 id
 * @param id_righ 赋值后的md5 id
 */
void AsignMd5Id(t_md5_id& id_left, const t_md5_id& id_right);

/**
 * @brief 将md5转换成md5 id
 * @param md5 md5
 * @param md5_id md5 id
 * @return 成功返回0，失败返回负数
 */
int ConvMd5_to_Id(const string& md5, t_md5_id& md5_id);

/**
 * @brief 将md5 id还原成md5
 * @param md5_id md5 id
 * @param buf 长度不小于33
 * @return 返回md5
 */
int ConvMd5Id_to_Md5(const t_md5_id& md5_id, char* buf);

/**
 * @brief 获取md5 id的hash值
 * @param md5_id 用户id
 * @param hash_table_len hash表的长度
 * @return user id的hash值
 */
int GetMd5HashCode(const t_md5_id& md5_id, int hash_table_len, int factor = 17);

// user id
/// @brief sogou user_id的转换类，sogou user id的特点为128位整数的16进制表示
///        每个user id的字符串长度为32，范围为A-F,0-9
#define USER_ID_LEN 16 
typedef unsigned char t_user_id[USER_ID_LEN];	// 用户id，用16各字节表示

/**
 * @brief 比较两个用户的大小
 * @param id1 用户1的id
 * @param id2 用户2的id
 * @return id1 < id2 返回负数
 *         id1 = id2 返回0
 *         id1 > id2 返回正数
 */
int CmpUserId(const t_user_id& id1, const t_user_id& id2);

/**
 * @brief 将一个user id赋值给另一个user id
 * @param id_left 被赋值的user id
 * @param id_righ 赋值后的user id
 */
void AsignUserId(t_user_id& id_left, const t_user_id& id_right);

/**
 * @brief 将用户名转换成用户id
 * @param user_name 用户名
 * @param user_id 用户id
 * @return 成功返回0，失败返回负数
 */
int ConvUserName_to_Id(const string& user_name, t_user_id& user_id);

/**
 * @brief 将用户名转换成用户id
 * @param user_id 用户id
 * @return 返回用户名
 */
string ConvUserId_to_Name(const t_user_id& user_id);

/**
 * @brief 判断用户ID是否为空
 * @param user_id 用户id
 * @return 如果用户ID为空，返回false
 */
bool IsZero(const t_user_id& user_id);

/**
 * @brief 获取user id的hash值
 * @param user_id 用户id
 * @param hash_table_len hash表的长度
 * @return user id的hash值
 */
int GetUserHashCode(const t_user_id& user_id, int hash_table_len, int factor = 17);
unsigned int GetUserCode(const t_user_id& user_id, int factor = 19);

/**
 * @brief 将userid转换成一个浮点数
 * @param userid 用户id，userid必须是合法的
 * @return 成功返回0，失败返回负数
 */
float GetUserNameWeight(const char* userid);

/// @brief IP地址的转换程序
/**
 * @brief 将IP的字符串表示转换成unsigned int表示
 * @param ip 字符表示的IP地址，"202.38.64.1"
 * @param value 输出变量，uint表示的IP地址
 * @return 0表示成功，失败返回负数
 */
int ConvIP_to_UInt(const char *ip, unsigned int& value);

/**
 * @brief 将IP的字符串表示转换成unsigned int表示
 * @param value uint表示的IP地址
 * @param ip 输出值，字符表示的IP地址，"202.38.64.1"，调用者分配空间
 * @return 0表示成功，失败返回负数
 */
int ConvUInt_to_IP(unsigned int value, char* ip);

/**
 * @brief 获取ip的hash值
 * @param ip ip的unsigned int表示
 * @param hash_table_len hash表的长度
 * @param factor hash因子
 * @return user id的hash值
 */
int GetIPHashCode(unsigned int ip, int hash_table_len, int factor = 17);

/// @brief url的64位hash值，用于判断两个url是否相等
/**
 * @brief 计算URL的hash码
 * @param url url
 * @return URL的hash码
 */
uint64_t GetUrlHash(const char* url, int factor = 17);

/**
 * @brief 对url进行处理，去掉分页符和参数
 * @param url 处理前的url
 * @return 返回处理后的url
 */
char* CheckUrl(char* url);

}
#endif

