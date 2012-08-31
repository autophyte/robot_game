#include "robotpool.h"



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
 
}

int pool_exit_thread() {

    return -1;
}

void pool_robotpool(struct robotpool *ppool) {
    int i;
    if (NULL!=ppool) {
        memset(ppool, 0, sizeof(struct robotpool));

        ppool->ids = 0;
        ppool->ncount = 0;
        sem_init(&ppool->sem_ncount, 0, 0);
        module_init_con(ppool, &ppool->pid_select);

        for (i=0; i<MAX_CLIENT; ++i) {
            rob_robot(&ppool->robots[i], ppool->ids++, i);
        }
    }
}

/**
 * 获取池中一个空的、没有使用的元素
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
            sem_wait(&ppool->sem_ncount);
            ++ppool->ncount;
            sem_post(&ppool->sem_ncount);
        }
    }

    return iret;
}


struct cconnect *pool_findcon_fd(void *vpool, int fd) {
    int i;
    struct cconnect *pret;
    struct robotpool *ppool;

    pret = NULL;
    ppool = (struct robotpool *)vpool;
    for (i=0; i<MAX_CLIENT; ++i) {
        if (fd == ppool->robots[i].con.sockfd) {
            pret = &ppool->robots[i].con;
            break;
        }
    }
    return pret;
}

struct robot *pool_findrob_fd(void *vpool, int fd) {
    int i;
    struct robot *pret;
    struct robotpool *ppool;

    pret = NULL;
    ppool = (struct robotpool *)vpool;
    for (i=0; i<MAX_CLIENT; ++i) {
        if (fd == ppool->robots[i].con.sockfd) {
            pret = &ppool->robots[i];
            break;
        }
    }

    return pret;
}