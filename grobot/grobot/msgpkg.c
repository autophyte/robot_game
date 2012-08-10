/*msgpkg.c*/
#include "msgpkg.h"
#include <string.h>

void msgpkg_constructor(struct msgpkg *ppkg) {
    memset(ppkg->msg, 0, sizeof(ppkg->msg));
    ppkg->msg_valid   = MSGPKG_VALID;
    ppkg->msg_len     = 0;
}

void msgpkg_constructor_arr(struct msgpkg *arr, _uint count) {
    _uint i;
    for (i=0; i<count; ++i) {
        msgpkg_constructor(arr+i);
    }
}
