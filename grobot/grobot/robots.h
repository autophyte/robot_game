#ifndef _ROBOTS_H_
#define _ROBOTS_H_  1
#include "connect.h"
#include "loger.h"

enum {
    ROBOT_FREE = 0,
    ROBOT_USED = 1
};

/**
 * 描述一个机器人
 */
struct robot {
    /* 机器人管理、内存分配 */
    int             index;              /**< 在数组中的编号 */
    int             id;                 /**< ID */
    int             valid;              /**< 1表示已经使用，0表示没有使用 */

    /* 网络 */
    struct cconnect con;                /**< connect */
    struct loger    log;                /**< 日志 */

    /* 线程相关 */
    pthread_t       pid_rob;            /**< 运行robot的线程PID */
    pthread_t       pid_con;            /**< 用于发送接收服务器端返回消息的线程 */
    pthread_mutex_t mutex;              /**< 互斥锁 */
    pthread_cond_t  cond;               /**< 条件变量 */
};


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
 * @see log_init_log con_init_cconnect
 */
void rob_robot(struct robot *prob, int ID, int idx, const char *ip, _ushort port);
int rob_start(struct robot *prob);

#endif /* _ROBOTS_H_ */

