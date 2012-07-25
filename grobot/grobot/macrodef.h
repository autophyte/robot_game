#ifndef _MACRODEF_H_
#define _MACRODEF_H_    1

/* macro: constant */

/**
 * 定议客户端IP地址
 */
#define CLIENTADDR      "127.0.0.1"

/**
 * 定议IP地址的最大长度
 */
#define ADDRESS_LEN     20

/**
 * 设定发送包的最大长度
 */
#define PACKAGE_LEN     1024

/**
 * 设定充许的客户端的最大数量
 */
#define MAX_CLIENT      128

/**
 *设定buffer的长度
 */
#define MAX_BUFFER      1024

#ifdef WIN32
/**
 * 统一windows和linux函数名称
 */
#define strncasecmp     strnicmp

/**
 * 定议windows下socket版本号
 */
#define SOCKET_VERSION  0x202
#endif /*WIN32*/

#ifndef __GNUC__
/**
 * GCC优化宏，非GCC时设定为空
 */
#define __attribute__(a)
#endif /*__attribute__*/

#ifndef MAX_PATH
/**
 * 定议文件名最长度
 */
#define MAX_PATH        260
#endif  /*MAX_PATH*/

/* macro: macro_functions */

/**
 * 记录是志：指针为空
 */
#define record_ptr(pcon) do {\
    printf("Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
    log_record(&pcon->log, "Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
} while (0)

/**
 * 记录是志：返回值为-1
 */
#define record_ret(pcon) do {\
    printf("Error: -1==ret in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
    log_record(&pcon->log, "Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
} while (0)

/**
 * 记录是志：将指定的参数记录
 */
#define record_ifo(pcon, ret) do {\
    printf(">>>>"##ret##"<<<<\n");\
    log_record(&pcon->log, ">>>>"##ret##"<<<<\n");\
}while(0)

/**
 * 标记参数没有使用
 */
#define unused(ret) do {\
} while (0)


#endif