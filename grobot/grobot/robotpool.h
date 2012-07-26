#ifndef _ROBOTPOOL_H_
#define _ROBOTPOOL_H_  1
#include "robots.h"


struct _tag_robotpool {
    robot       robots[MAX_CLIENT];     /**< robot������ */
    int         valids[MAX_CLIENT];     /**< �������Ӧ��pools[n]�Ƿ���Ч��1��Ч */
    int         ncount;                 /**< ��ǰ��Ϸ���� */
    int         ids;                    /**< ��¼��һ��robot��ID�� */
};
typedef struct _tag_robotpool robotpool;

/**
 * ��ʼ�������˳�
 *
 * @param[in] ppool ��Ҫ��ʼ���ĳ�
 *
 * @return
 */
int pool_init(robotpool *ppool);

/**
 * ����һ��������
 *
 * @param[in] ppool ��Ҫ���������˵ĳ�
 *
 * @return
 */
int pool_create_robot(robotpool *ppool);


#endif /* _ROBOTPOOL_H_ */

