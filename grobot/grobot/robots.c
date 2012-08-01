#include "robots.h"
#include <pthread.h>

extern pthread_mutex_t g_mutex_exit;
extern pthread_cond_t g_cond_exit;
extern pthread_mutex_t g_mutex_rcv;
extern pthread_cond_t g_cond_rcv;

void *rob_loop_l(void *ppar) {
    int iret;
    struct robot *prob = (struct robot *)ppar;
    if (NULL==prob) {
        iret = -1;
    }

    pthread_exit(&iret);
    return NULL;
}

int rob_start(struct robot *prob) {
    pthread_t tid;
    int iret=-1;
    if (NULL!=prob) {
        iret = pthread_create(&tid, NULL, rob_loop_l, (void *)prob);
        if (0==iret) {
            return 0;
        }
    }

    return -1;
}


void rob_robot(struct robot *prob,
    int ID, int idx, const char *ip, ushort port) {
        int iret;
        do {
            if (NULL==prob || ID <0 || idx <0 || idx >=MAX_CLIENT || NULL==ip) {
                printf("Error: init robot error, in function %s:%s, at line: %d\n",
                    __FILE__, __FUNCTION__, __LINE__);
                break;
            }

            log_loger((void *)prob, "f:\\log.txt");
            con_cconnect(&prob->con, (void *)prob);

            iret = con_setup(&prob->con, 0, ip, port, 1);
            if (0>iret) {
                record_ret(&prob->log);
                break;
            }

            iret = con_create_tcp(&prob->con);
            if (0>iret) {
                record_ret(&prob->log);
                break;
            }

            prob->mutex = PTHREAD_MUTEX_INITIALIZER;
            prob->cond  = PTHREAD_COND_INITIALIZER;
            iret = rob_start(prob);
            if (0>iret) {
                record_ret(&prob->log);
                break;
            }
        } while (0);
}

