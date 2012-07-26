#include "connect.h"
#include "macrodef.h"


static int g_current_connect_number = 0;


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


int con_init_cconnect(cconnect *p_con) {
    int iret=-1;
    do {
        if (NULL==p_con) {
            break;
        }

        memset(p_con, 0, sizeof(cconnect));
        iret = log_init_log(&p_con->log, "f:\\log.txt");
        if (0>iret) {
            printf("Error: create log file, in function %s:%s, at line: %d\n",
                __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        p_con->n_nonblock   = 1;
        p_con->n_index = g_current_connect_number++;

#ifdef WIN32
        iret = con_initwin();
        if (0>iret) {
            record_ifo(p_con, "select socket dll version error");
            break;
        }
#endif
        iret = 0;
    } while (0);

    return iret;
}

int con_set_tcp(cconnect *pcon, const char *szip, int nport, int nnblock) {
    if (NULL!=pcon) {
        pcon->sa_dst.sin_family         = htons(nport);
        pcon->sa_dst.sin_addr.s_addr    = inet_addr(szip);
        pcon->sa_loc.sin_family         = htons(0);
        pcon->sa_loc.sin_addr.s_addr    = inet_addr(CLIENTADDR);
        pcon->n_domain                  = AF_INET;
        pcon->n_type                    = SOCK_STREAM;
        pcon->n_nonblock                = nnblock;
        return 0;
    }
    return -1;
}

int con_set_udp(cconnect *pcon, const char *szip, int nport, int nnblock) {
    if (NULL!=pcon) {
        pcon->sa_dst.sin_family         = htons(nport);
        pcon->sa_dst.sin_addr.s_addr    = inet_addr(szip);
        pcon->sa_loc.sin_family         = htons(0);
        pcon->sa_loc.sin_addr.s_addr    = inet_addr(CLIENTADDR);
        pcon->n_domain                  = AF_INET;
        pcon->n_type                    = SOCK_DGRAM;
        pcon->n_nonblock                = nnblock;
        return 0;
    }
    return -1;
}

int con_set_block(cconnect *p_con, int nonblock) {
    int flags;

    if (0<=nonblock) {
        p_con->n_nonblock = nonblock;
    }
    if (NULL!=p_con && p_con->n_sockfd>0 &&
        (p_con->n_nonblock==0||p_con->n_nonblock==1)) {
#ifdef linux
        flags = fcntl(p_con->n_sockfd, F_GETFL, 0);
        if (0==p_con->n_nonblock) {
            flags &= ~O_NONEBLOCK;
        }
        else{
            flags |= O_NONEBLOCK;
        }
        fcntl(p_con->n_sockfd, F_SETFL, flags);
#else /*linux*/
        flags = p_con->n_nonblock;
        ioctlsocket(p_con->n_sockfd, FIONBIO, (u_long *)&flags);
#endif /*linux*/
        return 0;
    }
    return -1;
}

int con_create_tcp(cconnect *p_con) {
    int iret;

    do {
        iret = -1;
        record_ifo(p_con, "start tcp client");

        if (NULL==p_con){
            record_ptr(p_con);
            break;
        }

        record_ifo(p_con, "create socket");
        p_con->n_sockfd = socket(p_con->n_domain, p_con->n_type, 0);
        if (0>p_con->n_sockfd) {
            record_ret(p_con);
            break;
        }

        record_ifo(p_con, "bind local address and port");
        iret = bind(p_con->n_sockfd, (const struct sockaddr *)&p_con->sa_loc,
            sizeof(struct sockaddr));
        if (0>iret) {
            record_ret(p_con);
            break;
        }

        record_ifo(p_con, "start connect");
        iret = connect(p_con->n_sockfd, (const struct sockaddr *)&p_con->sa_dst,
            sizeof(struct sockaddr));
        if (0>iret) {
            record_ret(p_con);
            break;
        }

        iret = con_set_block(p_con, -1);
        if (0>iret) {
            record_ret(p_con);
            break;
        }
        record_ifo(p_con, "connect start successed");
        iret = 0;
    } while (0);

    return iret;
}