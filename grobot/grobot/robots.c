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

static char g_sz_local_path[MAX_PATH];

/* private functions */
static void rob_mklogpath(struct robot *prob) {
    
    getcwd(g_sz_local_path, sizeof(g_sz_local_path));
}

static void *rob_loop_l(void *ppar) {
    int iret;
    struct robot *prob = (struct robot *)ppar;
    if (NULL==prob) {
        iret = -1;
    }

    pthread_exit(&iret);
    return NULL;
}

/* public functions */
void rob_robot(struct robot *prob, int ID, int idx, const char *ip, ushort port) {
    if (NULL!=prob && ID >=0 && idx >=0 && NULL!=ip) {
        prob->mutex = PTHREAD_MUTEX_INITIALIZER;
        prob->cond  = PTHREAD_COND_INITIALIZER;
        prob->valid = ROBOT_FREE;
        log_loger(&prob->log, "f:\\log.txt");
        con_cconnect(&prob->con, (void *)prob);
        con_setup(&prob->con, 0, ip, port, 1);
    }
}

int rob_start(struct robot *prob) {
    int iret;
    pthread_t tid;

    do {
        iret=-1;

        if (NULL==prob){
            break;
        }

        iret = con_create_tcp(&prob->con);
        if (0>iret) {
            break;
        }

        iret = pthread_create(&tid, NULL, rob_loop_l, (void *)prob);
        if (0>iret) {
            break;
        }
        iret = 0;
    } while (0);

    return iret;
}



