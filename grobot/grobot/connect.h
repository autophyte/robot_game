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
 * ��������ΪTCP����UDP
 */
enum {
    CON_TCP         = 0,
    CON_UDP         = 1,
    CON_TCP_ERROR   = 2
};

/**
 * �������������ӻ��߷���������
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
    void *phost;                    /**< ���ڽṹ���ָ�� */
    int index;                      /**< connect��� */

    /* ���������Ϣ */
    struct sockaddr_in  sa_dst;     /**< �������˵�ַ */
    struct sockaddr_in  sa_loc;     /**< ���ص�ַ */
    int sockfd;
    int domain;
    int type;                       /**< �������� */
    int status;                     /**< ״̬��0��δ���ӣ�1���Ѿ����� */
    _uint nonblock;                 /**< ��������־��1����������0���� */

    struct loger log;               /**< ��־ */

    /**
     * ������Ϣֻ����һ�����������Ҫ����Ϣ����
     */
    struct msgs msg_r;
    struct msgs msg_s;

    struct rb_node node;         /**< ��ʾ���е�ǰ�ڵ� */
};



/**
 * ��ʼ��conģ��
 */
int module_init_con(void *ppool, pthread_t  *ppid);

/**
 * ��ȡһ����Ϣ�Ƿ���Ч����Ч����1�����򷵻�0
 */
int con_get_msgvalide(const struct msgpkg *pkg);

/**
 * �趨һ����Ϣ�Ƿ���Ч������valide��Ч1������0
 */
void con_set_msgvalide(struct msgpkg *pkg, int valide);

/**
 * ��ʼ��cconnect����
 *
 * @param[out] pcon ��Ҫ��ʼ����cconnect������ָ��
 * @param[in] phost �趨������ָ�룬��cconnect�������ڽṹ��ָ��
 *
 * @note ÿ��connect��������Ҫ���������ʼ������ʹ��
 */
void con_cconnect(struct cconnect *pcon, void *phost);


/**
 * ����tcp���Ĳ���
 *
 * @details ����udp/tcp���Ĳ�������������IP��ַ���˿ںţ�����IP��ַ���˿ںš��������͡�
 *      �����Ƿ�����
 *
 * @param[out] pcon ��Ҫ���õ�connect��ָ��
 * @param[in] ntype ��������,0:tcp, >0 udp
 * @param[in] szip ��������IP��ַ
 * @param[in] nport �������Ķ˿ں�
 * @param[in] nnblock ������ģʽ��־��1����������0����
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ�ܣ�������pconΪNULL���ȡ���ض˿ں�ʧ��
 *
 * @note �������ֻ�����ô���socket�Ĳ�����@link con_create_tcp @endlink
 *      �������������������ô���socket����
 *
 * @see con_getport_loc con_set_udp con_create_tcp
 */
int con_setup(struct cconnect *pcon,
    _uint ntype, const char *szip, _ushort nport, _uint nnblock);


/**
 * ����udp���Ĳ���
 *
 * @see con_getport_loc con_set_tcp con_create_udp
 */
int con_set_udp(struct cconnect *pcon,
    const char *szip, _uint nport, _uint nnblock);


/**
 * ����socket�Ƿ�����
 *
 * @param[in,out] pcon ��Ҫ���õ�connect��ָ��
 * @param[in] nonblock ������ģʽ��־��1����������0������2����pcon�еı�־����
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ�ܣ�������pconΪNULL��socket��Ч��������־��Ч
 *
 * @note ����������е�ǰ����socket�Ѿ��������������nonblock��Ϊ-1������nonblock
 *      �������Ƿ����������򣬰���pcon�еĲ�������
 *
 * @see con_create_tcp con_set_tcp con_set_udp
 */
int con_set_block(struct cconnect *pcon, _uint nonblock);


/**
 * ������Ϣ
 *
 * @param[in] pcon ��Ҫ������Ϣ��connect��ָ��
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ��
 *
 * @remark �͵���Ϣ��pcon�ṹ�ڲ��е���Ϣ������
 */
int con_snd_message(struct cconnect *pcon);


/**
 * ������Ϣ
 *
 * @param[in] pcon ������Ϣ��connect��ָ��
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ��
 *
 * @remark ���յ���Ϣ��pcon�ṹ�ڲ��е���Ϣ������
 */
int con_rcv_message(struct cconnect *pcon);


/**
 * ����һ��tcp����
 *
 * @details ��������pcon�еĲ�������һ��ͨѶ�����ӻ���socket->bind->connect�Ĳ���
 *      �����������Ƿ�����
 *
 * @param[in,out] p_con ��Ҫ�������ӵ�connect��ָ��
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ��
 */
int con_create_tcp(struct cconnect *p_con);


/**
 * �������緢�ͽ�����Ϣ���߳�������
 */
static void *con_net_man(void *ppool);

#endif /* _CONNECT_H_ */