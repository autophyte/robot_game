#include "connect.h"
#include "macrodef.h"


typedef int (*RCVSNDFUN)(struct cconnect *pcon);

static int              g_current_connect_number;
static volatile int     g_max_select_fd;
static fd_set           g_fd_read;
static fd_set           g_fd_write;

struct cconnect *pool_findcon_fd(void *ppool, int fd);
struct robot *pool_findrob_fd(void *ppool, int fd);

int module_init_con(void *ppool, pthread_t *ppid) {
    int iret;
    iret = -1;
    if (ppid && ppool) {
        g_current_connect_number    = 0;
        g_max_select_fd             = 0;
        FD_ZERO(&g_fd_read);
        FD_ZERO(&g_fd_write);
        iret = pthread_create(ppid, NULL, con_net_man, ppool);
        if (!iret) {
            iret = -1;
        }
    }
    return iret;
}


void con_add_select(struct cconnect *pcon) {
    if (NULL!=pcon) {
        FD_SET(pcon->sockfd, &g_fd_read);
        FD_SET(pcon->sockfd, &g_fd_write);
    }
}

void con_clr_select(struct cconnect *pcon) {
    if (NULL!=pcon) {
        FD_CLR(pcon->sockfd, &g_fd_read);
        FD_CLR(pcon->sockfd, &g_fd_write);
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

        pcon->phost        = phost;
        pcon->nonblock    = CON_NBLOCK;
        pcon->index       = g_current_connect_number++;
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
            pcon->domain                  = AF_INET;
            if (CON_TCP==ntype) {
                pcon->type                = SOCK_STREAM;
            }
            else {
                pcon->type                = SOCK_DGRAM;
            }
            pcon->nonblock                = nnblock;
            return 0;
        }
        return -1;
}

int con_set_block(struct cconnect *pcon, _uint nonblock) {
    int flags;

    if (0<=nonblock) {
        pcon->nonblock = nonblock;
    }
    if (NULL!=pcon && pcon->sockfd>0 && pcon->nonblock<CON_BLOCK_ERROR) {
#ifdef linux
        flags = fcntl(pcon->sockfd, F_GETFL, 0);
        if (CON_BLOCK==pcon->nonblock) {
            flags &= ~O_NONBLOCK;
        }
        else{
            flags |= O_NONBLOCK;
        }
        fcntl(pcon->sockfd, F_SETFL, flags);
#elif defined WIN32
        flags = pcon->nonblock;
        ioctlsocket(pcon->sockfd, FIONBIO, (u_long *)&flags);
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

        pcon->sockfd = socket(pcon->domain, pcon->type, 0);
        if (0>pcon->sockfd) {
            break;
        }

        con_set_maxfd(pcon->sockfd);
        iret = bind(pcon->sockfd, (const struct sockaddr *)&pcon->sa_loc,
            sizeof(struct sockaddr));
        if (0>iret) {
            break;
        }

        iret = connect(pcon->sockfd, (const struct sockaddr *)&pcon->sa_dst,
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
    int iret;
    struct msgpkg pkg;
    iret = -1;

    if (pcon) {
        iret = msgs_pop(&pcon->msg_r, &pkg);
        if (RETURN_ERROR!=iret) {
            iret = con_snd_send(pcon->sockfd, pkg.msg, pkg.msg_len);
        }
    }

    return iret;
}

int con_rcv_message(struct cconnect *pcon) {
    int iret;
    struct msgpkg pkg;

    iret = -1;
    if (pcon) {
        msgpkg_constructor(&pkg);
        iret = recv(pcon->sockfd, pkg.msg, sizeof(pkg.msg)-1, 0);

        if (RETURN_ERROR!=iret) {
            pkg.msg_len = iret;
            pkg.msg_valid = MSGPKG_VALID;
            iret = msgs_push(&pcon->msg_r, &pkg);
        }
    }

    return iret;
}


static void con_foreach_fds(void *pool,
    fd_set *pdes, fd_set *psrc, RCVSNDFUN fn) {
    _uint i;
    for (i=0; i<pdes->fd_count; ++i) {
        if (FD_ISSET(pdes->fd_array[i], psrc)) {
            fn(pool_findcon_fd(pool, pdes->fd_array[i]));
        }
    }
}

static void *con_net_man(void *ppool) {
    int ierror;
    fd_set  fd_read, fd_write;
    struct timeval fd_waite;

    fd_waite.tv_sec = 10;
    fd_waite.tv_usec = 0;

    for (;;) {
        con_load_selects(&fd_read, &fd_write);
        ierror = select(g_max_select_fd, &fd_read, &fd_write, NULL, &fd_waite);
        if (0==ierror) {
            continue;
        }
        if (RETURN_ERROR==ierror) {
            break;
        }
        con_foreach_fds(ppool, &g_fd_read, &fd_read, con_rcv_message);
        con_foreach_fds(ppool, &g_fd_write, &fd_write, con_snd_message);
    }

    return NULL;
}
