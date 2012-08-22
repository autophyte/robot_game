/* connect.h */
#ifndef _CONNECT_H_
#define _CONNECT_H_ 1

#ifdef linux
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <rbtree.h>
#elif defined WIN32
#include <WinSock2.h>
#include "rbtree.h"
#endif /*linux*/
#include <pthread.h>
#include "msgs.h"
#include "loger.h"



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

/**
 * describe connect
 */
struct cconnect {
    void *phost;                    /**< 所在结构体的指针 */
    int index;                      /**< connect编号 */

    /* 网络相关信息 */
    struct sockaddr_in  sa_dst;     /**< 服务器端地址 */
    struct sockaddr_in  sa_loc;     /**< 本地地址 */
    int sockfd;
    int domain;
    int type;                       /**< 连接类型 */
    int status;                     /**< 状态，0：未连接，1：已经连接 */
    _uint nonblock;                 /**< 非阻塞标志，1：非阻塞，0阻塞 */

    struct loger log;               /**< 日志 */

    /**
     * 现在消息只处理一个，后面可能要加消息缓存
     */
    struct msgs msg_r;
    struct msgs msg_s;

    struct rb_node node;         /**< 表示树中当前节点 */
};



/**
 * 初始化con模块
 */
int module_init_con(void *ppool, pthread_t  *ppid);

/**
 * 获取一个消息是否有效，有效返回1，否则返回0
 */
int con_get_msgvalide(const struct msgpkg *pkg);

/**
 * 设定一个消息是否有效，参数valide有效1，否则0
 */
void con_set_msgvalide(struct msgpkg *pkg, int valide);

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
 *
 * @retval 0 成功
 * @retval -1 函数运行失败
 *
 * @remark 送的消息在pcon结构内部中的消息链表中
 */
int con_snd_message(struct cconnect *pcon);


/**
 * 接收消息
 *
 * @param[in] pcon 接收消息的connect的指针
 *
 * @retval 0 成功
 * @retval -1 函数运行失败
 *
 * @remark 接收的消息在pcon结构内部中的消息链表中
 */
int con_rcv_message(struct cconnect *pcon);


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


/**
 * 处理网络发送接收消息的线程主函数
 */
static void *con_net_man(void *ppool);

/*
 * 将收发线程已经收取到的消息取出
 *
 * 机器人主线程调用
 */
#define con_pull_msg(pcon, pmsg) \
    msgs_pop(&((pcon)->msg_r), (struct msgpkg *)(pmsg))

/*
 * 将准备发送的消息压入消息队列
 *
 * 机器人主线程调用
 */
#define con_push_msg(pcon, pmsg) \
    msgs_push(&((pcon)->msg_r), (struct msgpkg *)(pmsg))

#endif /* _CONNECT_H_ */