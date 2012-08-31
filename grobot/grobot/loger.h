﻿#ifndef _LOGER_H_
#define _LOGER_H_   1

#include "pathstring.h"


/**
 * 描述一个日志文件
 */
struct loger
{
    FILE *p_file;
    char sz_name[MAX_PATH];
};
/**
 * 初始化日志文件目录
 */
int module_init_log();

/**
 * 初始化日志文件
 */
void log_loger(struct loger *plog, int ID);

/**
 * 结束日志文件
 */
int log_end_log(struct loger *plog);

/**
 * 记录日志文件
 */
int log_record(struct loger *plog, const char *fmt, ...)__attribute__(
    (format(printf,2,3)));


#endif /*_LOGER_H_*/


