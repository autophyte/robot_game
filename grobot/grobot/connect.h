/* connect.h */
#ifndef _CONNECT_H_
#define _CONNECT_H_ 1

#ifdef linux
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#else /*linux*/
#include <WinSock2.h>
#endif /*linux*/


#include "loger.h"


/**
 * describe connect
 */
struct __tag_cconnect {
    /* 网络相关信息 */
    struct sockaddr_in  sa_dst;     /**< 服务器端地址 */
    struct sockaddr_in  sa_loc;     /**< 本地地址 */
    int n_sockfd;                   /**< socket id */
    int n_domain;                   /**< 地址类型 */
    int n_type;                     /**< 连接类型 */
    int n_status;                   /**< 状态，0：未连接，1：已经连接 */
    int n_nonblock;                 /**< 非阻塞标志，1：非阻塞，0阻塞 */
    /* other */
    loger log;                      /**< 日志 */
    int n_index;                    /**< connect编号 */
};
typedef struct __tag_cconnect cconnect;
typedef cconnect *pconnect;


/**
 * 获取一个空的端口号
 *
 * @return 成功时返回端口号，或失败返回0
 * @deprecated
 */
unsigned short con_getport_loc();


/**
 * 初始化cconnect变量
 *
 * @param[out] p_con 需要初始化的cconnect变量的指针
 *
 * @retval 0 成功
 * @retval -1 初始化失败，可能是指针为空，或获取日志文件名出错，或创建log文件失败
 *
 * @note 每个connect变量都需要这个函数初始化后方能使用
 */
int con_init_cconnect(cconnect *p_con);


/**
 * 设置tcp连的参数
 *
 * @details 设置udp连的参数，包括服器IP地址、端口号，本机IP地址、端口号、连接类型、
 *      连接是否阻塞
 *
 * @param[out] pcon 需要设置的connect的指针
 * @param[in] szip 服务器的IP地址
 * @param[in] nport 服务器的端口号
 * @param[in] nnblock 非阻塞模式标志，1：非阻塞，0阻塞
 *
 * @retval 0 成功
 * @retval -1 函数运行失败，可能是pcon为NULL或获取本地端口号失败
 *
 * @note 这个函数只是设置创建socket的参数，@link con_create_tcp @endlink
 *      函数会根据这个参数设置创建socket连接
 *
 * @see con_getport_loc con_set_udp con_create_tcp
 */
int con_set_tcp(cconnect *pcon, const char *szip, int nport, int nnblock);


/**
 * 设置udp连的参数
 *
 * @see con_getport_loc con_set_tcp con_create_udp
 */
int con_set_udp(cconnect *pcon, const char *szip, int nport, int nnblock);


/**
 * 设置socket是否阻塞
 *
 * @param[in,out] pcon 需要设置的connect的指针
 * @param[in] nonblock 非阻塞模式标志，1：非阻塞，0阻塞，-1根据pcon中的标志设置
 *
 * @retval 0 成功
 * @retval -1 函数运行失败，可能是pcon为NULL或socket无效或阻塞标志无效
 *
 * @note 这个函数运行的前提是socket已经创建，如果参数nonblock不为-1，则按照nonblock
 *      来设置是否阻塞，否则，按照pcon中的参数设置
 *
 * @see con_create_tcp con_set_tcp con_set_udp
 */
int con_set_block(cconnect *pcon, int nonblock);


/**
 * 发送消息
 *
 * @param[in] pcon 需要发送消息的connect的指针
 * @param[in] pmsg 需要发送的消息
 * @param[in] nlen 需要发送的消息长度
 *
 * @retval 0 成功
 * @retval -1 函数运行失败
 */
int con_snd_msg(cconnect *pcon, void *pmsg, int nlen);


/**
 * 接收消息
 *
 * @param[in] pcon 接收消息的connect的指针
 * @param[out] pmsg 接收到的消息
 * @param[out] plen 接收到的消息的长度
 *
 * @retval 0 成功
 * @retval -1 函数运行失败
 */
int con_rcv_msg(cconnect *pcon, void *pmsg, int *plen);


/**
 * 创建一个tcp连接
 *
 * @details 函数会根椐pcon中的参数创建一个通讯的连接会有socket->bind->connect的步骤
 *      并会设置连是否阻塞
 *
 * @param[in,out] p_con 需要创建连接的connect的指针
 *
 * @retval 0 成功
 * @retval -1 函数运行失败
 */
int con_create_tcp(cconnect *p_con);
#endif /* _CONNECT_H_ */