#include "connect.h"
#include "macrodef.h"


static int              g_current_connect_number;
static volatile int     g_max_select_fd;
static fd_set           g_fd_read;
static fd_set           g_fd_write;

void module_init_con() {
    g_current_connect_number    = 0;
    g_max_select_fd             = 0;
    FD_ZERO(&g_fd_read);
    FD_ZERO(&g_fd_write);
}

void msg_msgpkg(struct msgpkg *pmsg) {
    if (NULL!=pmsg) {
        memset(pmsg->msg, 0, sizeof(pmsg->msg));
        pmsg->msg_valid = MSGPKG_INVALID;
        pmsg->msg_len   = 0;
    }
}

void con_add_select(struct cconnect *pcon) {
    if (NULL!=pcon) {
        FD_SET(pcon->n_sockfd, &g_fd_read);
        FD_SET(pcon->n_sockfd, &g_fd_write);
    }
}

void con_clr_select(struct cconnect *pcon) {
    if (NULL!=pcon) {
        FD_CLR(pcon->n_sockfd, &g_fd_read);
        FD_CLR(pcon->n_sockfd, &g_fd_write);
    }
}

void con_load_selects(fd_set *pread, fd_set *pwrite) {
    if (NULL!=pread) {
        memcpy(pread, (void *)&g_fd_read, sizeof(fd_set));
    }
    if (NULL!=pwrite) {
        memcpy(pwrite, (void *)&g_fd_write, sizeof(fd_set));
    }
}

void con_cconnect(struct cconnect *pcon, void *phost) {
    if (NULL!=pcon) {
        memset(pcon, 0, sizeof(struct cconnect));

        pcon->p_host        = phost;
        pcon->n_nonblock    = CON_NBLOCK;
        pcon->n_index       = g_current_connect_number++;
        msg_msgpkg(&pcon->msg_r);
        msg_msgpkg(&pcon->msg_s);
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
    _uint ntype, const char *szip, _ushort nport, _uint nnblock) {
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

int con_set_block(struct cconnect *pcon, _uint nonblock) {
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

int con_rcv_msg(struct cconnect *pcon, void *pmsg, _uint *plen) {
    int iret=-1;
    if (NULL!=pcon && NULL!=pmsg && *plen>0 || *plen<PACKAGE_LEN){
        iret = send(pcon->n_sockfd, pmsg, *plen, 0);
    }

    return iret;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>splite<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

int con_snd_send(int fd, char *pmsg, _uint nlen) {
    int ileft, iret, ipos;

    ipos = 0;
    iret = SOCKET_ERROR;
    for (ileft = nlen; ileft > 0; ileft -= iret) {
        iret = send(fd, pmsg+ipos, ileft, 0);
        if (SOCKET_ERROR==iret||0==iret) {
            break;
        }
        ipos += iret;
    }

    if (SOCKET_ERROR!=iret) {
        iret = 0;
    }

    return iret;
}

int con_snd_message(struct cconnect *pcon) {
    int iret, ileft;

    do {
        iret = -1;
        if (NULL==pcon) {
            break;
        }

        if (!con_get_msgvalide(&pcon->msg_s)){
            break;
        }

        iret = con_snd_send(
            pcon->n_sockfd, pcon->msg_s.msg, pcon->msg_s.msg_len);
        if (0>iret) {
            break;
        }
        con_set_msgvalide(&pcon->msg_s, MSGPKG_INVALID);
    } while (0);

    return iret;
}

int con_rcv_message(struct cconnect *pcon) {
    int iret;

    do {
        if (NULL!=pcon) {
            iret = -1;
            break;
        }
        /* 上一个消息有效，表示程序还没有处理上一个消息 */
        if (MSGPKG_VALID==con_get_msgvalide(&pcon->msg_r)){
            break;
        }
        memset(pcon->msg_r.msg, 0, sizeof(pcon->msg_r.msg));
        iret = recv(pcon->n_sockfd,
            pcon->msg_r.msg, sizeof(pcon->msg_r.msg)-1, 0);
        if (0>iret) {
            break;
        }
        pcon->msg_r.msg_len = iret;
        con_set_msgvalide(&pcon->msg_r, MSGPKG_VALID);
    } while (0);

    return iret;
}

void con_net_man() {
    fd_set  fd_read, fd_write;
    struct timeval fd_waite;
    _uint i;
    int ierror;

    fd_waite.tv_sec = 10;
    fd_waite.tv_usec = 0;

    for (;;) {
        con_load_selects(&fd_read, &fd_write);
        ierror = select(g_max_select_fd, &fd_read, &fd_write, NULL, &fd_waite);
        if (0==ierror) {
            continue;
        }
        if (-1==ierror) {
            break;
        }
        for (i=0; i<g_fd_read.fd_count; ++i) {
            if (FD_ISSET(g_fd_read.fd_array[i], &fd_read)) {
                /* send message to thread who's fd==g_fd_read.fd_array[i] */
            }
        }
        for (i=0; i<g_fd_write.fd_count; ++i) {
            if (FD_ISSET(g_fd_write.fd_array[i], &fd_write)) {
                /* send message to thread who's fd==g_fd_read.fd_array[i] */
            }
        }
    }
}
