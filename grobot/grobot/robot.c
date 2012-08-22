#include "robotpool.h"

//#define __TEST__
/* function: implementation */
#ifdef linux

#define con_initwin32sock(pret) do {\
} while (0)

#define con_dinitwin32sock() do {\
} while (0);

#elif defined WIN32

static void con_initwin32sock() {
    WSADATA wsaData;
    WSAStartup(SOCKET_VERSION, &wsaData);
}
#define con_dinitwin32sock() WSACleanup()

#endif /*linux*/


#ifndef __TEST__

int main(int argc, char *argv[]) {
    struct robotpool pool;
    con_initwin32sock();

    pool_robotpool(&pool);
    /*get_robot_count()*/
    /*set_robot_name()*/
    pool_new_robot(&pool);
    while(1) {
        if (pool.ncount) {
            sleep(1000);
            continue;
        }
        break;
    }
    con_dinitwin32sock();
    return 0;
}
#else /*__TEST__*/

int main(int argc, char *argv[]) {
    char sz_local_path[MAX_PATH];

    printf("test1\n");
    getcwd(sz_local_path, sizeof(sz_local_path));
    printf(sz_local_path);
    printf("test2\n");
    return 0;
}
#endif /*__TEST__*/
