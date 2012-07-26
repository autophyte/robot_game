#include "robotpool.h"

/* function: implementation */
#ifdef WIN32

static int con_initwin() {
    WSADATA wsaData;

    if (NO_ERROR!=WSAStartup(SOCKET_VERSION, &wsaData)) {
        return -1;
    }
    return 0;
}
#endif /*WIN32*/

int main(int argc, char *argv[]) {
    int iret;
    robotpool pool;
#ifdef WIN32
    iret = con_initwin();
#endif /* WIN32 */
    iret = pool_init(&pool);

#ifdef WIN32
    WSACleanup();
#endif /*WIN32*/
    return 0;
}