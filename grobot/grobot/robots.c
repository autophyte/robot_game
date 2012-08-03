#include "robots.h"
#include <pthread.h>

#ifdef linux
#include <unistd.h>
#else /*linux*/
#include <direct.h>
#endif /*linux*/

extern pthread_mutex_t g_mutex_exit;
extern pthread_cond_t g_cond_exit;
extern pthread_mutex_t g_mutex_rcv;
extern pthread_cond_t g_cond_rcv;


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

static void *rob_loop_con(void *ppar) {
    int i_ext;
    void *p_return;
    char sz_msg[PACKAGE_LEN];
    struct robot *prob;
    _uint nlen;

    i_ext       = -1;
    p_return    = NULL;
    prob        = (struct robot *)ppar;

    if (NULL!=prob) {
        for (;;) {
            memset(sz_msg, 0, sizeof(sz_msg));
            con_rcv_msg(&prob->con, sz_msg, &nlen);

        }
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

        iret = pthread_create(&prob->pid_con, NULL, rob_loop_con, (void *)prob);
        if (0>iret) {
            break;
        }
        iret = 0;
    } while (0);

    return iret;
}



