#ifndef _ROBOTPOOL_H_
#define _ROBOTPOOL_H_  1
#include "robots.h"


struct robotpool {
    /** robot的数组 */
    struct robot    robots[MAX_CLIENT];

    /** 描述相对应的pools[n]是否有效，1表示有效，没有被占用 */
    /*int             valids[MAX_CLIENT];*/

    /** 当前游戏数量 */
    int             ncount;

    /** 记录下一个robot的ID号 */
    int             ids;
};

/**
 * 初始化机器人池
 *
 * @param[in] ppool 需要初始化的池
 */
void pool_robotpool(struct robotpool *ppool);

/**
 * 在池中创建一个机器人
 *
 * @param[in] ppool 需要创建机器人的池
 *
 * @see pool_create_robot
 */
int pool_new_robot(struct robotpool *ppool);

/**
 * 通知主线程线程退出
 */
int pool_exit_thread();


#endif /* _ROBOTPOOL_H_ */

