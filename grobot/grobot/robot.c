#include "robotpool.h"

/* function: implementation */
#ifdef WIN32

static void con_initwin32sock() {
    WSADATA wsaData;
    WSAStartup(SOCKET_VERSION, &wsaData);
}
#define con_dinitwin32sock() WSACleanup()

#else /*WIN32*/

#define con_initwin32sock(pret) do {\
}while(0)

#define con_dinitwin32sock() do {\
} while (0);
 
#endif /*WIN32*/



int main(int argc, char *argv[]) {
    struct robotpool pool;
    con_initwin32sock();

    pool_robotpool(&pool);

    con_dinitwin32sock();
    return 0;
}