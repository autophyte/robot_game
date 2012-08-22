#include "robotpool.h"

/** @def SERVERADDR
 * ����server��IP��ַ
 */
#define SERVERADDR      "127.0.0.1"

/** @def SERVERPORT
 * ����server�˿�
 */
#define SERVERPORT      2208


/**
 * �ȴ��������߳��˳�
 *
 * @details �ȴ��������߳��˳�,ÿ�յ�һ�����߳��˳���Ϣ��g_cond_exit��,
 *      ����g_current_child_thread��1��ֱ��g_current_child_threadΪ0ʱ�˳�
 *
 * @note ������������̻߳��߹����̣߳�����й����̣߳��е��ã����ڵȴ����������߳�
 *      �˳��������߳���Ŷ�˳�ʱ����Ҫ����pool_exit_thread֪ͨ�߳̽���
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
 * ��ȡ����һ���յġ�û��ʹ�õ�Ԫ��
 *
 * @param[in] ppool �ص�ָ��
 *
 * @retval -1 ��ȡԪ��ʧ�ܣ�����ppoolΪ�գ������ǳ�����
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