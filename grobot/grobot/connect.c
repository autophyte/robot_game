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


int con_init_cconnect(cconnect *pcon, void *phost) {
    int iret=-1;
    do {
        if (NULL==pcon) {
            record_ptr(phost);
            break;
        }

        memset(pcon, 0, sizeof(cconnect));
/*
        iret = log_init_log(&pcon->log, "f:\\log.txt");
        if (0>iret) {
            printf("Error: create log file, in function %s:%s, at line: %d\n",
                __FILE__, __FUNCTION__, __LINE__);
            break;
        }
*/
        pcon->p_host = phost;
        pcon->n_nonblock   = 1;
        pcon->n_index = g_current_connect_number++;
/*
#ifdef WIN32
        iret = con_initwin();
        if (0>iret) {
            record_ifo(pcon->p_host, "select socket dll version error");
            break;
        }
#endif
        */
        iret = 0;
    } while (0);

    return iret;
}

int con_set_tcp(cconnect *pcon, const char *szip, ushort nport, int nnblock) {
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

int con_set_block(cconnect *pcon, int nonblock) {
    int flags;

    if (0<=nonblock) {
        pcon->n_nonblock = nonblock;
    }
    if (NULL!=pcon && pcon->n_sockfd>0 &&
        (pcon->n_nonblock==0||pcon->n_nonblock==1)) {
#ifdef linux
        flags = fcntl(pcon->n_sockfd, F_GETFL, 0);
        if (0==pcon->n_nonblock) {
            flags &= ~O_NONEBLOCK;
        }
        else{
            flags |= O_NONEBLOCK;
        }
        fcntl(pcon->n_sockfd, F_SETFL, flags);
#else /*linux*/
        flags = pcon->n_nonblock;
        ioctlsocket(pcon->n_sockfd, FIONBIO, (u_long *)&flags);
#endif /*linux*/
        return 0;
    }
    return -1;
}

int con_create_tcp(cconnect *pcon) {
    int iret;

    do {
        iret = -1;
        record_ifo(pcon->p_host, "start tcp client");
        if (NULL==pcon){
            record_ptr(pcon->p_host);
            break;
        }

        record_ifo(pcon->p_host, "create socket");
        pcon->n_sockfd = socket(pcon->n_domain, pcon->n_type, 0);
        if (0>pcon->n_sockfd) {
            record_ret(pcon->p_host);
            break;
        }

        record_ifo(pcon->p_host, "bind local address and port");
        iret = bind(pcon->n_sockfd, (const struct sockaddr *)&pcon->sa_loc,
            sizeof(struct sockaddr));
        if (0>iret) {
            record_ret(pcon->p_host);
            break;
        }

        record_ifo(pcon, "start connect");
        iret = connect(pcon->n_sockfd, (const struct sockaddr *)&pcon->sa_dst,
            sizeof(struct sockaddr));
        if (0>iret) {
            record_ret(pcon->p_host);
            break;
        }

        iret = con_set_block(pcon, -1);
        if (0>iret) {
            record_ret(pcon->p_host);
            break;
        }
        record_ifo(pcon->p_host, "connect start successed");
    } while (0);

    return iret;
}