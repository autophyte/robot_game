#ifndef _LOGER_H_
#define _LOGER_H_   1

#include "macrodef.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * ����һ����־�ļ�
 */
struct loger
{
    FILE *p_file;
    char sz_name[MAX_PATH];
};

/**
 * ��ʼ����־�ļ�
 */
void log_loger(struct loger *plog, const char *sz_filename);

/**
 * ������־�ļ�
 */
int log_end_log(struct loger *plog);

/**
 * ��¼��־�ļ�
 */
int log_record(struct loger *plog, const char *fmt, ...)__attribute__(
    (format(printf,2,3)));
#endif /*_LOGER_H_*/


