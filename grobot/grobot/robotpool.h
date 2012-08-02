#ifndef _ROBOTPOOL_H_
#define _ROBOTPOOL_H_  1
#include "robots.h"


struct robotpool {
    /** robot������ */
    struct robot    robots[MAX_CLIENT];

    /** �������Ӧ��pools[n]�Ƿ���Ч��1��ʾ��Ч��û�б�ռ�� */
    /*int             valids[MAX_CLIENT];*/

    /** ��ǰ��Ϸ���� */
    int             ncount;

    /** ��¼��һ��robot��ID�� */
    int             ids;
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

