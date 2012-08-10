#include "robots.h"
#include <pthread.h>

#ifdef linux
#include <unistd.h>
#elif defined WIN32
#include <direct.h>
#endif /*linux*/

/* private functions */
static void *rob_loop_main(void *ppar) {
    int i_ext;
    void *p_return;
    struct robot *prob = (struct robot *)ppar;
    if (NULL==prob) {
        i_ext = -1;
        p_return = NULL;
    }

    pthread_exit(&i_ext);
    return p_return;
}

/* public functions */
void rob_robot(struct robot *prob, int ID, int idx, const char *ip, _ushort port) {
    if (NULL!=prob && ID >=0 && idx >=0 && NULL!=ip) {
        prob->mutex = PTHREAD_MUTEX_INITIALIZER;
        prob->cond  = PTHREAD_COND_INITIALIZER;
        prob->valid = ROBOT_FREE;
        prob->id    = ID;
        prob->index = idx;

        log_loger(&prob->log, ID);
        con_cconnect(&prob->con, (void *)prob);
        con_setup(&prob->con, 0, ip, port, CON_NBLOCK);
    }
}

int rob_start(struct robot *prob) {
    int iret;

    do {
        iret=-1;

        if (NULL==prob){
            break;
        }

        iret = con_create_tcp(&prob->con);
        if (0>iret) {
            break;
        }

        iret = pthread_create(&prob->pid_rob, NULL, rob_loop_main, (void *)prob);
        if (0>iret) {
            break;
        }
        iret = 0;
    } while (0);

    return iret;
}



