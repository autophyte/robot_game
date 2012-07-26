#include "robots.h"


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
    } while (0);

    return iret;
}

