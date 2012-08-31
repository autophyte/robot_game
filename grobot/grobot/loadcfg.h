#ifndef _LOADCFG_H_
#define _LOADCFG_H_ 1

/**
 * assert lua script file exist
 *
 * @param[in] sz_file script file name
 *
 * @retval 0 file exist
 * @retval -1 file not exist
 */
#define lcfg_fileexist(sz_file) access(sz_file, 0)

/**
 * init load lua config module
 */
void module_init_lcfg();

/**
 * un-init load lua config module
 */
void module_uninit_lcfg();

/**
 * run a lua script file
 */
void lcfg_run_script(const char *sz_file);

/**
 * load a int value from lua script file
 *
 * @param[in] pla point lua interpreter
 * @pamra[in] name variable name
 * @param[out] pvl point of variable
 * @param[in] nlen the len of pvl(lcfg_get_str function only)
 *
 * @retval 0 function run success
 * @retval -1 some error occured
 */
int lcfg_get_int(void *pla, const char *name, int *pvl);
int lcfg_get_str(void *pla, const char *name, char *pvl, unsigned int nlen);
void *lcfg_create_interpreter(const char *szfile);
void lcfg_close_interpreter(void *pla);
#endif /*_LOADCFG_H_*/