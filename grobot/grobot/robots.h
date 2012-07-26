#ifndef _ROBOTS_H_
#define _ROBOTS_H_  1
#include "connect.h"
#include "loger.h"


/**
 * 描述一个机器人
 */
struct _tag_robot {
    int         id;                     /**< ID */
    int         index;                  /**< 在数组中的编号 */
    cconnect    con;                    /**< connect */
    loger       log;                    /**< 日志 */
};
typedef struct _tag_robot robot;


/**
 * 初始化一个机器人
 *
 * @details 初始化一个机器人，包括变量基本设置、以及日志初始化、connect模块初始化、
 *      其它模块初始化
 *
 * @param[in,out] prob 需要初始化的机器人的指针
 * @param[in] ID 设定机器人的ID号
 * @param[in] index 设定机器人在池中的index
 * @param[in] ip 设定服务器IP地址
 * @param[in] port 设定服务器端口号
 *
 * @retval -1 函数运行失，可能是参数错误或其它模块初始化失败导至
 * @retval 0 函数运行成功
 *
 * @see log_init_log con_init_cconnect
 */
int rob_init(robot *prob, int ID, int idx, const char *ip, ushort port);


#endif /* _ROBOTS_H_ */

