#include "robots.h"
#include <pthread.h>



void *rob_loop_l(void *ppar) {
    robot *prob = (robot *)ppar;
    if (NULL==prob) {
        return;
    }
}

int rob_start_l(robot *prob) {
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


int rob_init(robot *prob, int ID, int idx, const char *ip, ushort port) {
    int iret;

    do {
        iret = -1;
        if (NULL==prob || ID <0 || idx <0 || idx >=MAX_CLIENT || NULL==ip) {
            printf("Error: init robot error, in function %s:%s, at line: %d\n",
                __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        iret = log_init_log((void *)prob, "f:\\log.txt");
        if (0>iret) {
            printf("Error: create log file, in function %s:%s, at line: %d\n",
                __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        iret = con_init_cconnect(&prob->con, (void *)prob);
        if (0>iret) {
            record_ret((void *)prob);
            break;
        }

        iret = con_set_tcp(&prob->con, ip, port, 1);
        if (0>iret) {
            record_ret((void *)prob);
            break;
        }

        iret = con_create_tcp(&prob->con);
        if (0>iret) {
            record_ret((void *)prob);
            break;
        }

        prob->mutex = PTHREAD_MUTEX_INITIALIZER;
        prob->cond  = PTHREAD_COND_INITIALIZER;
    } while (0);

    return iret;
}

