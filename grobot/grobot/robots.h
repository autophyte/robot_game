#ifndef _ROBOTS_H_
#define _ROBOTS_H_  1
#include "connect.h"
#include "loger.h"

enum {
    ROBOT_FREE = 0,
    ROBOT_USED = 1
};

/**
 * ����һ��������
 */
struct robot {
    /* �����˹����ڴ���� */
    int             index;              /**< �������еı�� */
    int             id;                 /**< ID */
    int             valid;              /**< 1��ʾ�Ѿ�ʹ�ã�0��ʾû��ʹ�� */

    /* ���� */
    struct cconnect con;                /**< connect */
    struct loger    log;                /**< ��־ */

    /* �߳���� */
    pthread_t       pid_rob;            /**< ����robot���߳�PID */
    pthread_t       pid_con;            /**< ���ڷ��ͽ��շ������˷�����Ϣ���߳� */
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
void rob_robot(struct robot *prob, int ID, int idx, const char *ip, _ushort port);
int rob_start(struct robot *prob);

#endif /* _ROBOTS_H_ */

