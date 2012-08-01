#ifndef _ROBOTS_H_
#define _ROBOTS_H_  1
#include "connect.h"
#include "loger.h"



/**
 * ����һ��������
 */
struct robot {
    /* �����˹����ڴ���� */
    int             index;              /**< �������еı�� */
    int             id;                 /**< ID */

    /* ���� */
    struct cconnect con;                /**< connect */
    struct loger    log;                /**< ��־ */

    /* �߳���� */
    pthread_t       pid;                /**< ����robot���߳�PID */
    pthread_mutex_t mutex;              /**< ������ */
    pthread_cond_t  cond;               /**< �������� */
};


/**
 * ��ʼ��һ��������
 *
 * @details ��ʼ��һ�������ˣ����������������á��Լ���־��ʼ����connectģ���ʼ����
 *      ����ģ���ʼ��
 *
 * @param[in,out] prob ��Ҫ��ʼ���Ļ����˵�ָ��
 * @param[in] ID �趨�����˵�ID��
 * @param[in] index �趨�������ڳ��е�index
 * @param[in] ip �趨������IP��ַ
 * @param[in] port �趨�������˿ں�
 *
 * @see log_init_log con_init_cconnect
 */
void rob_robot(struct robot *prob, int ID, int idx, const char *ip, ushort port);


#endif /* _ROBOTS_H_ */

