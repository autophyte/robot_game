#ifndef _MACRODEF_H_
#define _MACRODEF_H_    1

/* macro: constant */

/**
 * ����ͻ���IP��ַ
 */
#define CLIENTADDR      "127.0.0.1"

/**
 * ����IP��ַ����󳤶�
 */
#define ADDRESS_LEN     20

/**
 * �趨���Ͱ�����󳤶�
 */
#define PACKAGE_LEN     1024

/**
 * �趨����Ŀͻ��˵��������
 */
#define MAX_CLIENT      128

/**
 *�趨buffer�ĳ���
 */
#define MAX_BUFFER      1024

#ifdef WIN32
/**
 * ͳһwindows��linux��������
 */
#define strncasecmp     strnicmp

/**
 * ����windows��socket�汾��
 */
#define SOCKET_VERSION  0x202
#endif /*WIN32*/

#ifndef __GNUC__
/**
 * GCC�Ż��꣬��GCCʱ�趨Ϊ��
 */
#define __attribute__(a)
#endif /*__attribute__*/

#ifndef MAX_PATH
/**
 * �����ļ������
 */
#define MAX_PATH        260
#endif  /*MAX_PATH*/

/* macro: macro_functions */

/**
 * ��¼��־��ָ��Ϊ��
 */
#define record_ptr(phost) do {\
    printf("Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
    log_record(phost, "Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
} while (0)

/**
 * ��¼��־������ֵΪ-1
 */
#define record_ret(phost) do {\
    printf("Error: -1==ret in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
    log_record(phost, "Error: NULL==ptr in function %s:%s, at line: %d\n",\
    __FILE__, __FUNCTION__, __LINE__);\
} while (0)

/**
 * ��¼��־����ָ���Ĳ�����¼
 */
#define record_ifo(phost, ret) do {\
    printf(">>>>"##ret##"<<<<\n");\
    log_record(phost, ">>>>"##ret##"<<<<\n");\
}while(0)

/**
 * ��ǲ���û��ʹ��
 */
#define unused(ret) do {\
} while (0)


#endif