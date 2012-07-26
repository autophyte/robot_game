#ifndef _ROBOTS_H_
#define _ROBOTS_H_  1
#include "connect.h"
#include "loger.h"


/**
 * ����һ��������
 */
struct _tag_robot {
    int         id;                     /**< ID */
    int         index;                  /**< �������еı�� */
    cconnect    con;                    /**< connect */
    loger       log;                    /**< ��־ */
};
typedef struct _tag_robot robot;


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
 * @retval -1 ��������ʧ�������ǲ������������ģ���ʼ��ʧ�ܵ���
 * @retval 0 �������гɹ�
 *
 * @see log_init_log con_init_cconnect
 */
int rob_init(robot *prob, int ID, int idx, const char *ip, ushort port);


#endif /* _ROBOTS_H_ */

