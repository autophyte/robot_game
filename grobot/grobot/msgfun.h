#ifndef _MSGFUN_H_
#define _MSGFUN_H_    1

typedef int (*ms_fun)(void *, int);
struct _tag_msgfun 
{
    unsigned int    n_msg;
    ms_fun          p_fun;
};
#endif /*_MSG_FUN_H_*/