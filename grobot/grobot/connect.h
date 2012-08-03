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

enum {
    MSGPKG_INVALID  = 0,
    MSGPKG_VALID    = 1,
    MSGPKG_ERROR    = 2
};

/**
 * ����һ���������յ�����Ϣ������׼�����͵��������Ϣ��
 */
struct msgpkg {
    char    msg[PACKAGE_LEN];   /**< ��Ϣ */
    _u8     msg_valid;          /**< ��Ϣ��û�б������ */
    _uint   msg_len;            /**< ��Ϣ��Ч���� */
};

/**
 * describe connect
 */
struct cconnect {
    /* ���������Ϣ */
    struct sockaddr_in  sa_dst;     /**< �������˵�ַ */
    struct sockaddr_in  sa_loc;     /**< ���ص�ַ */
    int n_sockfd;                   /**< socket id */
    int n_domain;                   /**< ��ַ���� */
    int n_type;                     /**< �������� */
    int n_status;                   /**< ״̬��0��δ���ӣ�1���Ѿ����� */
    _uint n_nonblock;               /**< ��������־��1����������0���� */
    /* other */
    void *p_host;                   /**< ���ڽṹ���ָ�� */
    struct loger log;               /**< ��־ */
    int n_index;                    /**< connect��� */
    /* msg informations */
    struct msgpkg msg_r;            /**< �յ�����Ϣ */
    struct msgpkg msg_s;            /**< Ҫ���͵���Ϣ */
};



/**
 * ��ʼ��conģ��
 */
void module_init_con();

/**
 * ��ȡһ����Ϣ�Ƿ���Ч����Ч����1�����򷵻�0
 */
int con_get_msgvalide(const struct msgpkg *pkg);

/**
 * �趨һ����Ϣ�Ƿ���Ч������valide��Ч1������0
 */
void con_set_msgvalide(const struct msgpkg *pkg, int valide);

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
 * @param[in] pmsg ��Ҫ���͵���Ϣ
 * @param[in] nlen ��Ҫ���͵���Ϣ����
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ��
 */
int con_snd_msg(struct cconnect *pcon, void *pmsg, int nlen);


/**
 * ������Ϣ
 *
 * @param[in] pcon ������Ϣ��connect��ָ��
 * @param[out] pmsg ���յ�����Ϣ
 * @param[out] plen ���յ�����Ϣ�ĳ���
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ��
 */
int con_rcv_msg(struct cconnect *pcon, void *pmsg, _uint *plen);


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



#endif /* _CONNECT_H_ */