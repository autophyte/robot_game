#include "loger.h"
#include "macrodef.h"
#include "robots.h"
#include <stdarg.h>


/**
 * 全局变量：程序所在路径
 */
struct pathstring g_path_app;
struct pathstring g_path_log;
extern struct pathstring g_path_cfg;

char g_sz_local_path[MAX_PATH];

#ifdef linux
const char DIR_CAT2[PEND_SIZE]  = "log/";
#elif defined WIN32
const char DIR_CAT2[PEND_SIZE]  = "log\\";
#endif /*linux*/



int module_init_log() {
    int iret;
    size_t n_len;

    iret = pst_init(&g_path_app);
    if (!iret) {
        pst_app_new(g_path_app, DIR_CAT2, g_path_log, iret);
    }

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
