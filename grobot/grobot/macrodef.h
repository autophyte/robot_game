#ifndef _MACRODEF_H_
#define _MACRODEF_H_    1

/* macro: constant */

/** @def CLIENTADDR
 * 定议客户端IP地址
 */
#define CLIENTADDR      "127.0.0.1"



/** @def ADDRESS_LEN
 * 定议IP地址的最大长度
 */
#define ADDRESS_LEN     20



/** @def MAX_CLIENT
 * 设定充许的客户端的最大数量
 */
#define MAX_CLIENT      128



/** @def MAX_BUFFER
 *设定buffer的长度
 */
#define MAX_BUFFER      1024



/** @def MAX_STOREMSG
 * 定议存储消息的数量
 */
#define MAX_STOREMSG    10



#ifdef WIN32
/** @def strncasecmp
 * 统一windows和linux函数名称
 */
#define strncasecmp     strnicmp

/** @def SOCKET_VERSION
 * 定议windows下socket版本号
 */
#define SOCKET_VERSION  0x202
#endif /*WIN32*/



/** @def SOCKET_ERROR
 * 定义socket函数返回错误值
 */
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif /*SOCKET_ERROR*/

#define RETURN_ERROR (-1)

/** @def __attribute__
 * GCC优化宏，非GCC时设定为空
 */
#ifndef __GNUC__
#define __attribute__(a)
#endif /*__attribute__*/



/**
 * 定义日志文件目录名称预留长度，预留给文件名
 */
#define PEND_SIZE    20



#ifndef MAX_PATH
/** @def MAX_PATH
 * 定议文件名最长度
 */
#define MAX_PATH        260
#endif  /*MAX_PATH*/



/* macro: macro_functions */

/** @def record_ret(phost)
 * 记录是志：返回值为-1
 */
#define record_ret(plog) do {\
    printf("Error: -1==ret in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
    log_record(plog, "Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
} while (0)



/** @def unused(phost)
 * 标记参数没有使用
 */
#define unused(ret) do {\
} while (0)



#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  



/** @def get_host
 * 根据成员变量的指针获其宿主变量的指针
 *
 * @param ptr 成员变量的指针
 * @param type 宿主类型
 * @param member 成员变量的名称
 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#ifndef NULL
#define NULL (void *)0
#endif

#endif