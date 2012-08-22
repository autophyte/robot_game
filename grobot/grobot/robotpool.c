#include "robotpool.h"

/** @def SERVERADDR
 * 瀹氳server绔疘P鍦板潃
 */
#define SERVERADDR      "127.0.0.1"

/** @def SERVERPORT
 * 瀹氳server绔彛
 */
#define SERVERPORT      2208


/**
 * 绛夊緟鎵€鏈夊瓙绾跨▼閫€鍑?
 *
 * @details 绛夊緟鎵€鏈夊瓙绾跨▼閫€鍑?姣忔敹鍒颁竴涓瓙绾跨▼閫€鍑烘秷鎭紙g_cond_exit锛?
 *      鍙橀噺g_current_child_thread鍑?锛岀洿鍒癵_current_child_thread涓?鏃堕€€鍑?
 *
 * @note 杩欎釜鍑芥暟鍦ㄤ富绾跨▼鎴栬€呯鐞嗙嚎绋嬶紙濡傛灉鏈夌鐞嗙嚎绋嬶級涓皟鐢紝鐢ㄤ簬绛夊緟鎵€鏈夊叾瀹冪嚎绋?
 *      閫€鍑猴紝鍏跺畠绾跨▼鍦ㄥ摝閫€鍑烘椂锛岄渶瑕佽皟鐢╬ool_exit_thread閫氱煡绾跨▼缁撴潫
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

        for (i=0; i<MAX_CLIENT; ++i) {
            rob_robot(&ppool->robots[i], ppool->ids++, i, SERVERADDR, SERVERPORT);
        }
    }
}

/**
 * 鑾峰彇姹犱腑涓€涓┖鐨勩€佹病鏈変娇鐢ㄧ殑鍏冪礌
 *
 * @param[in] ppool 姹犵殑鎸囬拡
 *
 * @retval -1 鑾峰彇鍏冪礌澶辫触锛屾垨璁竝pool涓虹┖锛屾垨璁告槸姹犲凡婊?
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