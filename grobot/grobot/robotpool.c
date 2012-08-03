#include "robotpool.h"

/** @def SERVERADDR
 * 定议server端IP地址
 */
#define SERVERADDR      "127.0.0.1"

/** @def SERVERPORT
 * 定议server端口
 */
#define SERVERPORT      2208

pthread_mutex_t g_mutex_exit;           /**< 互斥锁,用于通知子线程退出 */
pthread_cond_t g_cond_exit;             /**< 条件变量,用于通知子线程退出 */
pthread_mutex_t g_mutex_rcv;            /**< 互斥锁,用于通知子线程接收数据 */
pthread_cond_t g_cond_rcv;              /**< 条件变量,用于通知子线程接收数据 */
int g_current_child_thread;             /**< 记录当前还在运行的子线程 */

/**
 * 等待所有子线程退出
 *
 * @details 等待所有子线程退出,每收到一个子线程退出消息（g_cond_exit）,
 *      变量g_current_child_thread减1，直到g_current_child_thread为0时退出
 *
 * @note 这个函数在主线程或者管理线程（如果有管理线程）中调用，用于等待所有其它线程
 *      退出，其它线程在哦退出时，需要调用pool_exit_thread通知线程结束
 *
 * @see pool_exit_thread
 */
static void pool_wait_threads() {
    int iret;

    while(1) {
        pthread_mutex_lock(&g_mutex_exit);
        iret = pthread_cond_wait(&g_cond_exit, &g_mutex_exit);
        pthread_mutex_unlock(&g_mutex_exit);

        if (0!=iret) {
            continue;
        }

        --g_current_child_thread;
        if (0==g_current_child_thread) {
            break;
        }
    }
}

static void *pool_start_select(void *ppar) {

}
int pool_exit_thread() {
    if (0==pthread_cond_signal(&g_cond_exit)) {
        return 0;
    }

    return -1;
}

void pool_robotpool(struct robotpool *ppool) {
    int i;
    if (NULL!=ppool) {
        memset(ppool, 0, sizeof(struct robotpool));

        ppool->ids = 0;
        ppool->ncount = 0;

        g_current_child_thread  = 0;
        g_mutex_exit            = PTHREAD_MUTEX_INITIALIZER;
        g_cond_exit             = PTHREAD_COND_INITIALIZER;
        g_mutex_rcv             = PTHREAD_MUTEX_INITIALIZER;
        g_cond_rcv              = PTHREAD_COND_INITIALIZER;

        for (i=0; i<MAX_CLIENT; ++i) {
            rob_robot(&ppool->robots[i], ppool->ids++, i, SERVERADDR, SERVERPORT);
        }
    }
}

/**
 * 获取池中一个空的，没有使用的元素
 *
 * @param[in] ppool 池的指针
 *
 * @retval -1 获取元素失败，或许ppool为空，或许是池已满
 */
static int pool_get_freeele(struct robotpool *ppool) {
    int i, iret;

    do {
        iret = -1;
        if (NULL==ppool) {
            break;
        }

        for (i=0; i<MAX_CLIENT; ++i) {
            if (ROBOT_FREE==ppool->robots[i].valid) {
                iret = i;
                break;/* break for */
            }
        }
    } while (0);

    return iret;
}

int pool_new_robot(struct robotpool *ppool) {
    int ifree, iret;

    iret = -1;
    if (NULL!=ppool) {
        ifree = pool_get_freeele(ppool);

        if (0 < ifree) {
            ppool->robots[ifree].valid = ROBOT_USED;
            iret = rob_start(&ppool->robots[ifree]);
        }
    }

    return iret;
}


