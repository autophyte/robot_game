#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include "loadcfg.h"
#include "pathstring.h"

#pragma comment(lib, "lua51.lib")

extern unsigned int    g_ADDRESS_LEN;
extern unsigned int    g_MAX_CLIENT;
extern unsigned int    g_MAX_BUFFER;
extern unsigned int    g_MAX_STOREMSG;
extern unsigned int    g_PEND_SIZE;


struct pathstring g_path_cfg;

#define lua_main_cfg    "cfg_main.lua"



void module_init_lcfg() {
    void *pla;
    struct pathstring sz_cfg_main;
    int iret;

    pla = NULL;
    do {
        pst_app_new(g_path_cfg, lua_main_cfg, sz_cfg_main, iret);
        if (0>iret) {
            break;
        }

        pla = lcfg_create_interpreter(sz_cfg_main.sz_path);
        if (!pla) {
            break;
        }
    } while (0);

    lcfg_close_interpreter(pla);
}

void module_uninit_lcfg() {

}


void lcfg_run_script(const char *sz_file) {

}


void *lcfg_create_interpreter(const char *szfile) {
    lua_State *L;

    if (!lcfg_fileexist(szfile)) {
        L= lua_open();
        luaL_openlibs(L);

        if (!luaL_dofile(L, szfile)) {
            return (void *)L;
        }
    }

    return NULL;
}

void lcfg_close_interpreter(void *pla) {
    if (pla) {
        lua_close((lua_State *)pla);
    }
}

int lcfg_get_int(void *pla, const char *name, int *pvl) {
    int iret=-1;
    if (pla && name && pvl) {
        lua_getglobal((lua_State *)pla, name);
        if (lua_isnumber((lua_State *)pla, -1)) {
            *pvl = (int)lua_tonumber((lua_State *)pla, -1);
            iret = 0;
        }
    }
    return iret;
}

int lcfg_get_str(void *pla, const char *name, char *pvl, unsigned int nlen) {
    const char *pres;
    size_t nsize;

    int iret=-1;
    if (pla && name && pvl) {
        lua_getglobal((lua_State *)pla, name);
        if (lua_isstring((lua_State *)pla, -1)) {
            pres = lua_tostring((lua_State *)pla, -1);
            nsize = strlen(pres);
            if (nsize<nlen) {
                strcpy(pvl, pres);
                iret = 0;
            }
        }
    }
    return iret;
}