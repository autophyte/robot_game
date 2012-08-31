#ifndef _PATHSTRING_H_
#define _PATHSTRING_H_ 1
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macrodef.h"

/**
 * a string type which will store the path
 */
struct pathstring 
{
    char sz_path[MAX_PATH]; /**< string */
    size_t nsize;           /**< string len */
};

/**
 * init a pathstring variable
 *
 * @param[out] pstr the point of variable which will be construct
 *
 * retval 0 function run success
 * retval -1 sone error occured
 */
int pst_init(struct pathstring *pstr);

/**
 * append a string to the source path string
 *
 * @param[in,out] src the path string
 * @param[in] app the append string
 *
 * retval 0 function run success
 * retval -1 sone error occured
 */
int pst_app(struct pathstring *src, const char *app);

/**
 * append a string and create a new string
 *
 * @param[in] src the source string
 * @param[in] the string which will be append to src
 * @param[out] des the result
 * @param[out] iret return value
 */
#define pst_app_new(src, app, des, iret) do {\
    (des) = (src);\
    iret = pst_app(&(des), app);\
} while (0)


#endif
