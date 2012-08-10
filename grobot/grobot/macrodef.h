#ifndef _MACRODEF_H_
#define _MACRODEF_H_    1

/* macro: constant */

/** @def CLIENTADDR
 * ����ͻ���IP��ַ
 */
#define CLIENTADDR      "127.0.0.1"



/** @def ADDRESS_LEN
 * ����IP��ַ����󳤶�
 */
#define ADDRESS_LEN     20



/** @def MAX_CLIENT
 * �趨����Ŀͻ��˵��������
 */
#define MAX_CLIENT      128



/** @def MAX_BUFFER
 *�趨buffer�ĳ���
 */
#define MAX_BUFFER      1024



/** @def MAX_STOREMSG
 * ����洢��Ϣ������
 */
#define MAX_STOREMSG    10



#ifdef WIN32
/** @def strncasecmp
 * ͳһwindows��linux��������
 */
#define strncasecmp     strnicmp

/** @def SOCKET_VERSION
 * ����windows��socket�汾��
 */
#define SOCKET_VERSION  0x202
#endif /*WIN32*/



/** @def SOCKET_ERROR
 * ����socket�������ش���ֵ
 */
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif /*SOCKET_ERROR*/

#define RETURN_ERROR (-1)

/** @def __attribute__
 * GCC�Ż��꣬��GCCʱ�趨Ϊ��
 */
#ifndef __GNUC__
#define __attribute__(a)
#endif /*__attribute__*/



/**
 * ������־�ļ�Ŀ¼����Ԥ�����ȣ�Ԥ�����ļ���
 */
#define PEND_SIZE    20



#ifndef MAX_PATH
/** @def MAX_PATH
 * �����ļ������
 */
#define MAX_PATH        260
#endif  /*MAX_PATH*/



/* macro: macro_functions */

/** @def record_ret(phost)
 * ��¼��־������ֵΪ-1
 */
#define record_ret(plog) do {\
    printf("Error: -1==ret in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
    log_record(plog, "Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
} while (0)



/** @def unused(phost)
 * ��ǲ���û��ʹ��
 */
#define unused(ret) do {\
} while (0)



#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)  



/** @def get_host
 * ���ݳ�Ա������ָ���������������ָ��
 *
 * @param ptr ��Ա������ָ��
 * @param type ��������
 * @param member ��Ա����������
 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#ifndef NULL
#define NULL (void *)0
#endif

#endif