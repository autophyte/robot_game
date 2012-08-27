#include "robots.h"
#include <pthread.h>
#include "msgfun.h"
#include "connect.h"

#ifdef linux
#include <unistd.h>
#elif defined WIN32
#include <direct.h>
#endif /*linux*/

/* private functions */
static void *rob_loop_main(void *ppar) {
    int i_ext, i;
    void *p_return;
    struct msgpkg tmp_msg;
    struct _tag_msgfun mfarr[MAX_PATH];
    struct robot *prob = (struct robot *)ppar;
/*
    if (NULL==prob) {
        i_ext = -1;
        p_return = NULL;
    }*/
    /*rob_get_msgfun_ls(mfarr);*/
    msgpkg_constructor(&tmp_msg);
    for (i=0; i<MAX_PATH; ++i) {
        if (mfarr[i].n_msg) {
            con_push_msg(&prob->con, &tmp_msg);
            con_pull_msg(&prob->con, &tmp_msg);
            mfarr[i].p_fun(tmp_msg.msg, 0);
        }
    }
    /*pthread_exit(&i_ext);*/
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
        con_setup(&prob->con, 0, ip, port, CON_BK_NBLOCK);
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



