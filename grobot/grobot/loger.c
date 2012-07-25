#include "loger.h"
#include "macrodef.h"
#include <stdarg.h>

int log_init_log(loger *plog, const char *sz_filename) {
    int iret;
    size_t ilen;

    do {
        iret = -1;
        if (NULL==plog || NULL==sz_filename) {
            break;
        }

        ilen = strlen(sz_filename);
        if (MAX_PATH<=ilen) {
            break;
        }

        memcpy(plog->sz_name, sz_filename, ilen);
        plog->p_file = fopen(sz_filename, "wt");
        if (NULL==plog->p_file) {
            break;
        }
        iret = 0;
    } while (0);

    return iret;
}

int log_end_log(loger *plog) {
    if (NULL!=plog && NULL!=plog->p_file) {
        fclose(plog->p_file);
        plog->p_file = 0;
        return 0;
    }
    return -1;
}

int log_record(loger *plog, const char *fmt, ...) {
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
