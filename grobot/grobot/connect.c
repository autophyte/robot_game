#include "connect.h"
#include "macrodef.h"


static int              g_current_connect_number    =   0;
static volatile int     g_max_select_fd             =   0;

void con_cconnect(struct cconnect *pcon, void *phost) {
    if (NULL!=pcon) {
        memset(pcon, 0, sizeof(struct cconnect));

        pcon->p_host        = phost;
        pcon->n_nonblock    = CON_NBLOCK;
        pcon->n_index       = g_current_connect_number++;
    }
}

/**
 * 设置select的最大的fd号
 *
 * @param[in] fd 要根全局数比较的fd号
 *
 * @note 这个函数因为会更改全局变量的值，只在在主线程中调用，其它线程中只读全局变量
 */
static void con_set_maxfd(int fd) {
    if (fd >= g_max_select_fd) {
        g_max_select_fd = fd+1;
    }
}

int con_setup(struct cconnect *pcon,
    unsigned int ntype, const char *szip, ushort nport, unsigned int nnblock) {
        if (NULL!=pcon) {
            pcon->sa_dst.sin_family         = htons(nport);
            pcon->sa_dst.sin_addr.s_addr    = inet_addr(szip);
            pcon->sa_loc.sin_family         = htons(0);
            pcon->sa_loc.sin_addr.s_addr    = inet_addr(CLIENTADDR);
            pcon->n_domain                  = AF_INET;
            if (CON_TCP==ntype) {
                pcon->n_type                = SOCK_STREAM;
            }
            else {
                pcon->n_type                = SOCK_DGRAM;
            }
            pcon->n_nonblock                = nnblock;
            return 0;
        }
        return -1;
}

int con_set_block(struct cconnect *pcon, unsigned int nonblock) {
    int flags;

    if (0<=nonblock) {
        pcon->n_nonblock = nonblock;
    }
    if (NULL!=pcon && pcon->n_sockfd>0 && pcon->n_nonblock<CON_BLOCK_ERROR) {
#ifdef linux
        flags = fcntl(pcon->n_sockfd, F_GETFL, 0);
        if (CON_BLOCK==pcon->n_nonblock) {
            flags &= ~O_NONBLOCK;
        }
        else{
            flags |= O_NONBLOCK;
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

int con_create_tcp(struct cconnect *pcon) {
    int iret;

    do {
        iret = -1;
        if (NULL==pcon){
            break;
        }

        pcon->n_sockfd = socket(pcon->n_domain, pcon->n_type, 0);
        if (0>pcon->n_sockfd) {
            break;
        }

        con_set_maxfd(pcon->n_sockfd);
        iret = bind(pcon->n_sockfd, (const struct sockaddr *)&pcon->sa_loc,
            sizeof(struct sockaddr));
        if (0>iret) {
            break;
        }

        iret = connect(pcon->n_sockfd, (const struct sockaddr *)&pcon->sa_dst,
            sizeof(struct sockaddr));
        if (0>iret) {
            break;
        }

        iret = con_set_block(pcon, CON_BLOCK_ERROR);
        if (0>iret) {
            break;
        }
    } while (0);

    return iret;
}

int con_snd_msg(struct cconnect *pcon, void *pmsg, int nlen) {
    int iret=-1;
    if (NULL!=pcon && NULL!=pmsg && nlen>0 || nlen<PACKAGE_LEN){
        iret = send(pcon->n_sockfd, pmsg, nlen, 0);
    }

    return iret;
}

int con_rcv_msg(struct cconnect *pcon, void *pmsg, int *plen) {
    int iret=-1;
    /*if (NULL!=pcon && NULL!=pmsg && *nlen>0 || *plen<PACKAGE_LEN){
        iret = send(pcon->n_sockfd, pmsg, nlen, 0);
    }*/

    return iret;
}

