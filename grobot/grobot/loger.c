#include "loger.h"
#include "macrodef.h"
#include "robots.h"
#include <stdarg.h>

static char g_sz_local_path[MAX_PATH];

#ifdef linux
const char DIR_SPL              = '/';
const char DIR_CAT[PEND_SIZE]   = "/log/";
const char DIR_CAT2[PEND_SIZE]  = "log/";
#elif defined WIN32
const char DIR_SPL              = '\\';
const char DIR_CAT[PEND_SIZE]   = "\\log\\";
const char DIR_CAT2[PEND_SIZE]  = "log\\";
#endif /*linux*/

int module_init_log() {
    int iret;
    size_t n_len;

    iret = -1;
    memset(g_sz_local_path, 0, sizeof(g_sz_local_path) - PEND_SIZE);
    do {
        if (NULL==getcwd(g_sz_local_path, sizeof(g_sz_local_path))) {
            break;
        }

        n_len = sizeof(g_sz_local_path);
        /* 因为后面要加上["log\","\log\","/log/","log/"]中1个
         * 又要加上文件名"log_xxx.log"
         * 所以要预留1部分空间，保留长度为20
         */
        if (n_len>=MAX_PATH - PEND_SIZE) {
            break;
        }

        if (DIR_SPL!=g_sz_local_path[n_len-1]) {
            strcat(g_sz_local_path, DIR_CAT);
            break;
        }
        strcat(g_sz_local_path, DIR_CAT2);

        iret = 0;
    } while (0);

    return iret;
}

/* private functions */
static void log_mk_logname(int idx, char *sz_logname) {
    if (NULL!=sz_logname) {
        strncpy(sz_logname, g_sz_local_path, strlen(g_sz_local_path));
        sprintf(sz_logname+sizeof(sz_logname), "log_%d.txt", idx);
    }
}

/* public functions */
void log_loger(struct loger *plog, int ID) {
    if (NULL!=plog && 0< ID) {
        memset(plog, 0, sizeof(struct loger));
        log_mk_logname(ID, plog->sz_name);
        plog->p_file = fopen(plog->sz_name, "wt");
    }
}

int log_end_log(struct loger *plog) {
    if (NULL!=plog && NULL!=plog->p_file) {
        fclose(plog->p_file);
        plog->p_file = NULL;

        return 0;
    }
    return -1;
}

int log_record(struct loger *plog, const char *fmt, ...) {
    va_list valist;
    char buff[MAX_BUFFER];

    if (NULL!=plog || NULL!=plog->p_file || NULL!=fmt) {
        memset(buff, 0, MAX_BUFFER);

        va_start(valist, fmt);
        vsnprintf(buff, MAX_BUFFER, fmt, valist);
        va_end(valist);

        fprintf(plog->p_file, "%s\n", buff);
        return 0;
    }
    return -1;
}
