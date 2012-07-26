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
    /* ���������Ϣ */
    struct sockaddr_in  sa_dst;     /**< �������˵�ַ */
    struct sockaddr_in  sa_loc;     /**< ���ص�ַ */
    int n_sockfd;                   /**< socket id */
    int n_domain;                   /**< ��ַ���� */
    int n_type;                     /**< �������� */
    int n_status;                   /**< ״̬��0��δ���ӣ�1���Ѿ����� */
    int n_nonblock;                 /**< ��������־��1����������0���� */
    /* other */
    void *p_host;                   /**< ���ڽṹ���ָ�� */
    loger log;                      /**< ��־ */
    int n_index;                    /**< connect��� */
};
typedef struct __tag_cconnect cconnect;
typedef cconnect *pconnect;
typedef unsigned short ushort;


/**
 * ��ȡһ���յĶ˿ں�
 *
 * @return �ɹ�ʱ���ض˿ںţ���ʧ�ܷ���0
 * @deprecated
 */
unsigned short con_getport_loc();


/**
 * ��ʼ��cconnect����
 *
 * @param[out] pcon ��Ҫ��ʼ����cconnect������ָ��
 * @param[in] phost �趨������ָ�룬��cconnect�������ڽṹ��ָ��
 *
 * @retval 0 �ɹ�
 * @retval -1 ��ʼ��ʧ�ܣ�������ָ��Ϊ�գ����ȡ��־�ļ��������򴴽�log�ļ�ʧ��
 *
 * @note ÿ��connect��������Ҫ���������ʼ������ʹ��
 */
int con_init_cconnect(cconnect *pcon, void *phost);


/**
 * ����tcp���Ĳ���
 *
 * @details ����udp���Ĳ�������������IP��ַ���˿ںţ�����IP��ַ���˿ںš��������͡�
 *      �����Ƿ�����
 *
 * @param[out] pcon ��Ҫ���õ�connect��ָ��
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
int con_set_tcp(cconnect *pcon, const char *szip, ushort nport, int nnblock);


/**
 * ����udp���Ĳ���
 *
 * @see con_getport_loc con_set_tcp con_create_udp
 */
int con_set_udp(cconnect *pcon, const char *szip, int nport, int nnblock);


/**
 * ����socket�Ƿ�����
 *
 * @param[in,out] pcon ��Ҫ���õ�connect��ָ��
 * @param[in] nonblock ������ģʽ��־��1����������0������-1����pcon�еı�־����
 *
 * @retval 0 �ɹ�
 * @retval -1 ��������ʧ�ܣ�������pconΪNULL��socket��Ч��������־��Ч
 *
 * @note ����������е�ǰ����socket�Ѿ��������������nonblock��Ϊ-1������nonblock
 *      �������Ƿ����������򣬰���pcon�еĲ�������
 *
 * @see con_create_tcp con_set_tcp con_set_udp
 */
int con_set_block(cconnect *pcon, int nonblock);


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
int con_snd_msg(cconnect *pcon, void *pmsg, int nlen);


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
int con_rcv_msg(cconnect *pcon, void *pmsg, int *plen);


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
int con_create_tcp(cconnect *p_con);

/**
 * ����select������fd��
 *
 * @param[in] fd Ҫ��ȫ�����Ƚϵ�fd��
 *
 * @note ���������Ϊ�����ȫ�ֱ�����ֵ��ֻ�������߳��е��ã������߳���ֻ��ȫ�ֱ���
 */
void con_set_maxfd(int fd);
#endif /* _CONNECT_H_ */