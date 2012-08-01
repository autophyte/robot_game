#ifndef _ROBOTPOOL_H_
#define _ROBOTPOOL_H_  1
#include "robots.h"


struct robotpool {
    struct robot    robots[MAX_CLIENT];     /**< robot������ */
    int             valids[MAX_CLIENT];     /**< �������Ӧ��pools[n]�Ƿ���Ч��1��Ч */
    int             ncount;                 /**< ��ǰ��Ϸ���� */
    int             ids;                    /**< ��¼��һ��robot��ID�� */
};

/**
 * ��ʼ�������˳�
 *
 * @param[in] ppool ��Ҫ��ʼ���ĳ�
 */
void pool_robotpool(struct robotpool *ppool);

/**
 * �ڳ��д���һ��������
 *
 * @param[in] ppool ��Ҫ���������˵ĳ�
 *
 * @see pool_create_robot
 */
int pool_new_robot(struct robotpool *ppool);

/**
 * ֪ͨ���߳��߳��˳�
 */
int pool_exit_thread();


#endif /* _ROBOTPOOL_H_ */

