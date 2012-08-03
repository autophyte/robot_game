/* connect.h */
#ifndef _CONNECT_H_
#define _CONNECT_H_ 1

#ifdef linux
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#else /*linux*/
#include <WinSock2.h>
#endif /*linux*/


#include "loger.h"
#include <pthread.h>


typedef unsigned char       _u8;
typedef unsigned short      _ushort;
typedef unsigned short      _u16;
typedef unsigned int        _uint;

/**
 * 连接类型为TCP或者UDP
 */
enum {
    CON_TCP         = 0,
    CON_UDP         = 1,
    CON_TCP_ERROR   = 2
};

/**
 * 连接是阻塞连接或者非阻塞连接
 */
enum {
    CON_BLOCK       = 0,
    CON_NBLOCK      = 1,
    CON_BLOCK_ERROR = 2
};

enum {
    MSGPKG_INVALID  = 0,
    MSGPKG_VALID    = 1,
    MSGPKG_ERROR    = 2
};

/**
 * 描述一个从网络收到的消息包或者准备发送到网络的消息包
 */
struct msgpkg {
    char    msg[PACKAGE_LEN];   /**< 消息 */
    _u8     msg_valid;          /**< 消息有没有被处理过 */
    _uint   msg_len;            /**< 消息有效长度 */
};

/**
 * describe connect
 */
struct cconnect {
    /* 网络相关信息 */
    struct sockaddr_in  sa_dst;     /**< 服务器端地址 */
    struct sockaddr_in  sa_loc;     /**< 本地地址 */
    int n_sockfd;                   /**< socket id */
    int n_domain;                   /**< 地址类型 */
    int n_type;                     /**< 连接类型 */
    int n_status;                   /**< 状态，0：未连接，1：已经连接 */
    _uint n_nonblock;               /**< 非阻塞标志，1：非阻塞，0阻塞 */
    /* other */
    void *p_host;                   /**< 所在结构体的指针 */
    struct loger log;               /**< 日志 */
    int n_index;                    /**< connect编号 */
    /* msg informations */
    struct msgpkg msg_r;            /**< 收到的消息 */
    struct msgpkg msg_s;            /**< 要发送的消息 */
};



/**
 * 初始化con模块
 */
void module_init_con();

/**
 * 获取一个消息是否有效，有效返回1，否则返回0
 */
int con_get_msgvalide(const struct msgpkg *pkg);

/**
 * 设定一个消息是否有效，参数valide有效1，否则0
 */
void con_set_msgvalide(const struct msgpkg *pkg, int valide);

/**
 * 初始化cconnect变量
 *
 * @param[out] pcon 需要初始化的cconnect变量的指针
 * @param[in] phost 设定宿主的指针，即cconnect变量所在结构体指针
 *
 * @note 每个connect变量都需要这个函数初始化后方能使用
 */
void con_cconnect(struct cconnect *pcon, void *phost);


/**
 * 设置tcp连的参数
 *
 * @details 设置udp/tcp连的参数，包括服器IP地址、端口号，本机IP地址、端口号、连接类型、
 *      连接是否阻塞
 *
 * @param[out] pcon 需要设置的connect的指针
 * @param[in] ntype 连接类型,0:tcp, >0 udp
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
int con_setup(struct cconnect *pcon,
    _uint ntype, const char *szip, _ushort nport, _uint nnblock);


/**
 * 设置udp连的参数
 *
 * @see con_getport_loc con_set_tcp con_create_udp
 */
int con_set_udp(struct cconnect *pcon,
    const char *szip, _uint nport, _uint nnblock);


/**
 * 设置socket是否阻塞
 *
 * @param[in,out] pcon 需要设置的connect的指针
 * @param[in] nonblock 非阻塞模式标志，1：非阻塞，0阻塞，2根据pcon中的标志设置
 *
 * @retval 0 成功
 * @retval -1 函数运行失败，可能是pcon为NULL或socket无效或阻塞标志无效
 *
 * @note 这个函数运行的前提是socket已经创建，如果参数nonblock不为-1，则按照nonblock
 *      来设置是否阻塞，否则，按照pcon中的参数设置
 *
 * @see con_create_tcp con_set_tcp con_set_udp
 */
int con_set_block(struct cconnect *pcon, _uint nonblock);


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
int con_snd_msg(struct cconnect *pcon, void *pmsg, int nlen);


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
int con_rcv_msg(struct cconnect *pcon, void *pmsg, _uint *plen);


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
int con_create_tcp(struct cconnect *p_con);



#endif /* _CONNECT_H_ */