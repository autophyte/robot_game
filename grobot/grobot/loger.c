#include "loger.h"
#include "macrodef.h"
#include "robots.h"
#include <stdarg.h>

void log_loger(struct loger *plog, const char *sz_filename) {
    size_t ilen;

    if (NULL!=plog && NULL!=sz_filename) {
        memset(plog, 0, sizeof(struct loger));

        ilen = strlen(sz_filename);
        if (MAX_PATH>ilen) {
            memcpy(plog->sz_name, sz_filename, ilen);
            plog->p_file = fopen(sz_filename, "wt");
        }
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
