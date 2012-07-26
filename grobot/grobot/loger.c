#include "loger.h"
#include "macrodef.h"
#include "robots.h"
#include <stdarg.h>

int log_init_log(void *probot, const char *sz_filename) {
    int iret;
    size_t ilen;
    robot *p_robot = (robot *)probot;

    do {
        iret = -1;
        if (NULL==p_robot || NULL==sz_filename) {
            break;
        }

        ilen = strlen(sz_filename);
        if (MAX_PATH<=ilen) {
            break;
        }

        memcpy(p_robot->log.sz_name, sz_filename, ilen);
        p_robot->log.p_file = fopen(sz_filename, "wt");
        if (NULL==p_robot->log.p_file) {
            break;
        }
        iret = 0;
    } while (0);

    return iret;
}

int log_end_log(void *probot) {
    robot *p_robot = (robot *)probot;
    if (NULL!=p_robot && NULL!=p_robot->log.p_file) {
        fclose(p_robot->log.p_file);
        p_robot->log.p_file = 0;
        return 0;
    }
    return -1;
}

int log_record(void *probot, const char *fmt, ...) {
    va_list valist;
    char buff[MAX_BUFFER];
    robot *p_robot = (robot *)probot;

    if (NULL!=p_robot || NULL!=p_robot->log.p_file || NULL!=fmt) {
        memset(buff, 0, MAX_BUFFER);

        va_start(valist, fmt);
        vsnprintf(buff, MAX_BUFFER, fmt, valist);
        va_end(valist);

        fprintf(p_robot->log.p_file, "%s\n", buff);
        return 0;
    }
    return -1;
}
