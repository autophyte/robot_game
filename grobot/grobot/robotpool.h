#ifndef _ROBOTPOOL_H_
#define _ROBOTPOOL_H_  1
#include "robots.h"


struct _tag_robotpool {
    robot       robots[MAX_CLIENT];     /**< robot的数组 */
    int         valids[MAX_CLIENT];     /**< 描述相对应的pools[n]是否有效，1有效 */
    int         ncount;                 /**< 当前游戏数量 */
    int         ids;                    /**< 记录下一个robot的ID号 */
};
typedef struct _tag_robotpool robotpool;

/**
 * 初始化机器人池
 *
 * @param[in] ppool 需要初始化的池
 *
 * @return
 */
int pool_init(robotpool *ppool);

/**
 * 创建一个机器人
 *
 * @param[in] ppool 需要创建机器人的池
 *
 * @return
 */
int pool_create_robot(robotpool *ppool);


#endif /* _ROBOTPOOL_H_ */

