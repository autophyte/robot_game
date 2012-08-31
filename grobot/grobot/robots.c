#include "robots.h"
#include <pthread.h>
#include "msgfun.h"
#include "connect.h"

#ifdef linux
#include <unistd.h>
#elif defined WIN32
#include <direct.h>
#endif /*linux*/

/** @def SERVERADDR
 * 定议server端IP地址
 */
#define SERVERADDR      "127.0.0.1"

/** @def SERVERPORT
 * 定议server端口
 */
#define SERVERPORT      2208

#define ASSERTBREAK2(cret, bval) if(bval==cret) break
#define ASSERTBREAKI(iret, ivalue) if((ivalue)>(iret)) break

/* private functions */
static void *rob_loop_main(void *ppar) {
    int i;
    void *p_return;
    struct msgpkg tmp_msg;
    struct _tag_msgfun mfarr[MAX_PATH];
    struct robot *prob = (struct robot *)ppar;
    p_return = NULL;
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
void rob_robot(struct robot *prob, int ID, int idx) {
    if (NULL!=prob && ID >=0 && idx >=0) {
        prob->mutex = PTHREAD_MUTEX_INITIALIZER;
        prob->cond  = PTHREAD_COND_INITIALIZER;
        prob->valid = ROBOT_FREE;
        prob->id    = ID;
        prob->index = idx;

        log_loger(&prob->log, ID);
    }
}

int rob_set_con(struct robot *prob, const char *ip, _ushort port) {
    int iret = -1;
    if (prob && ip) {
        con_cconnect(&prob->con, (void *)prob);
        iret = con_setup(&prob->con, 0, ip, port, CON_BK_NBLOCK);
    }

    return iret;
}

#define load_config(prob, tp) do {} while (0)
/**
 * connect a server
 *
 * @detail this function will load connect config, and connect to server
 *
 * @param prob the point of the robot
 * @param iret the return value
 */
#define rob_connect_to(prob, iret) do {\
    load_config(prob, 0);\
    iret = rob_set_con((prob), SERVERADDR, SERVERPORT);\
    if (!iret) {\
        iret = con_create_tcp(&(prob)->con);\
    }\
} while (0)

/**
 * re-connect to a server
 *
 * @detail this function will stop a connect,
 *      load new config and connect to new server
 *
 * @param prob the point of the robot
 * @param iret the return value
 */
#define rob_reconnect_to(prob, iret) do {\
    iret = con_stop_con(&(prob)->con);\
    if (!iret) {\
        load_config(prob, 0);\
        rob_connect_to(prob, iret);\
    }\
} while (0)

int rob_start(struct robot *prob) {
    int iret;

    do {
        iret=-1;
        ASSERTBREAK2(prob, NULL);

        rob_connect_to(prob, iret);
        ASSERTBREAKI(iret, 0);

        iret = pthread_create(
            &prob->pid_rob, NULL, rob_loop_main, (void *)prob);
        ASSERTBREAKI(iret, 0);

        iret = 0;
    } while (0);

    return iret;
}



