#include "pathstring.h"
#ifdef linux
#include <unistd.h>
#else
#include <direct.h>
#endif

#ifdef linux
const char DIR_SPL              = '/';
#elif defined WIN32
const char DIR_SPL              = '\\';
#endif /*linux*/

int pst_init(struct pathstring *pstr) {
    int iret = -1;
    if (pstr) {
        memset(pstr, 0, sizeof(pstr->sz_path));
        /* 因为后面要加上["log\","\log\","/log/","log/","\cfg\","/cfg/"]等
         * 又要加上文件名"log_xxx.log"，"cfg_xxx.lua"等
         * 所以要预留1部分空间，保留长度为30
         */
        if (NULL!=getcwd(pstr->sz_path, sizeof(pstr->sz_path)-PEND_SIZE)) {
            pstr->nsize = strlen(pstr->sz_path);
            if (DIR_SPL!=pstr->sz_path[pstr->nsize]) {
                pstr->sz_path[pstr->nsize] = DIR_SPL;
            }
            iret = 0;
        }
    }

    return iret;
}

int pst_app(struct pathstring *src, const char *app) {
    size_t nlen;
    int iret = -1;
    if (src && app) {
        nlen = strlen(app);
        if (nlen+src->nsize<MAX_PATH) {
            strcat(src->sz_path, app);
            src->nsize += nlen;
            iret = 0;
        }
    }
    return iret;
}

