/*msgpkg.h*/
#ifndef _MSGPKG_H_
#define _MSGPKG_H_ 1

typedef unsigned char       _u8;
typedef unsigned short      _ushort;
typedef unsigned short      _u16;
typedef unsigned int        _uint;

/**
 * 描述一个从网络收到的消息包或者准备发送到网络的消息包
 */
struct msgpkg {
#define PACKAGE_LEN     (1024)
    char    msg[PACKAGE_LEN];   /**< 消息 */
#define MSGPKG_INVALID  (0)
#define MSGPKG_VALID    (1)
    _u8     msg_valid;          /**< 消息有没有被处理过 */
    _uint   msg_len;            /**< 消息有效长度 */
};

void msgpkg_constructor(struct msgpkg *this);
void msgpkg_constructor_arr(struct msgpkg *arr, _uint count);
#define msgpkg_copy(des, src)\
    memcpy((des), (src), sizeof(struct msgpkg))

#define msgpkg_mk_valide(src) do {src.msg_valid = MSGPKG_VALID} while (0)
#define msgpkg_cl_valide(src) do {src.msg_valid = MSGPKG_INVALID} while (0)

#endif /*_MSGPKG_H_*/
