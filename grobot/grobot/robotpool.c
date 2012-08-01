#include "robotpool.h"

/** @def SERVERADDR
 * ����server��IP��ַ
 */
#define SERVERADDR      "127.0.0.1"

/** @def SERVERPORT
 * ����server�˿�
 */
#define SERVERPORT      2208

pthread_mutex_t g_mutex_exit;           /**< ������,����֪ͨ���߳��˳� */
pthread_cond_t g_cond_exit;             /**< ��������,����֪ͨ���߳��˳� */
pthread_mutex_t g_mutex_rcv;            /**< ������,����֪ͨ���߳̽������� */
pthread_cond_t g_cond_rcv;              /**< ��������,����֪ͨ���߳̽������� */
int g_current_child_thread;             /**< ��¼��ǰ�������е����߳� */

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
        for (i=0; i<MAX_CLIENT; ++i) {
            ppool->valids[i]=1;
        }
        ppool->ids = 0;
        ppool->ncount = 0;

        g_current_child_thread  = 0;
        g_mutex_exit            = PTHREAD_MUTEX_INITIALIZER;
        g_cond_exit             = PTHREAD_COND_INITIALIZER;
        g_mutex_rcv             = PTHREAD_MUTEX_INITIALIZER;
        g_cond_rcv              = PTHREAD_COND_INITIALIZER;
    }
}

/**
 * ��ȡ����һ���յģ�û��ʹ�õ�Ԫ��
 *
 * @param[in] ppool �ص�ָ��
 *
 * @retval -1 ��ȡԪ��ʧ�ܣ�����ppoolΪ�գ������ǳ�����
 */
static int pool_get_freeele(struct robotpool *ppool) {
    int i=-1;

    if (NULL!=ppool) {
        for (i=0; i<MAX_CLIENT; ++i) {
            if (0==ppool->valids[i]) {
                break;
            }
        }
    }

    if (MAX_CLIENT <= i) {
        i=-1;
    }

    return i;
}

int pool_new_robot(struct robotpool *ppool) {
    int ifree, iret;

    do {
        iret = -1;
        if (NULL==ppool) {
            break;
        }

        ifree = pool_get_freeele(ppool);
        if (0>ifree) {
            break;
        }

        rob_robot(&ppool->robots[ifree], ppool->ids, ifree,
            SERVERADDR, SERVERPORT);
        ppool->valids[ifree] = 0;
    } while (0);

    return iret;
}


