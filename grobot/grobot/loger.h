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
struct _tag_loger
{
    FILE *p_file;
    char sz_name[MAX_PATH];
};
typedef struct _tag_loger loger;

/**
 * ��ʼ����־�ļ�
 */
int log_init_log(void *probot, const char *sz_filename);

/**
 * ������־�ļ�
 */
int log_end_log(void *probot);

/**
 * ��¼��־�ļ�
 */
int log_record(void *probot, const char *fmt, ...)
    __attribute__((format(printf,2,3)));
#endif /*_LOGER_H_*/


