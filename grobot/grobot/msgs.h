/*msgs.h*/
#ifndef _MSGS_H_
#define _MSGS_H_ 1

#include <semaphore.h>
#include "msgpkg.h"

/**
 * message array package
 */
struct msgs {
    struct msgpkg   *buf;
    _uint           max_slots;  /**< max slots count, set when msgs init */
    _uint           front;      /**< last data is: buf[front%max_slots] */
    _uint           rear;       /**< first data is: buf[rear%max_slots] */
    sem_t           mutex;      /**< mutex, protecte "buf" */
    sem_t           slots;      /**< current free slots count */
    sem_t           items;      /**< stored data count */
};

/**
 * construct a msgs array package
 *
 * @param[in,out] pmsgs point of package which will be constructed
 * @param[in] count the max count which the package can stored
 *
 * @note this function will call "malloc" function, so when no longer need
 *  this package, should call @link msgs_destructor @endlink function to
 *  destructor the package
 *
 * @see msgs_destructor
 *
 * @remark this function should be the first operate function for a package
 */
void msgs_constructor(struct msgs *pmsgs, _uint count);

/**
 * destruct a msgs array package
 *
 * @param[in, out] pmsgs point of package which will be destructed
 *
 * @see msgs_constructor
 *
 * @remark after called this function, the package can not be used, until user
 *  call function @link msgs_constructor @endlink again.
 */
void msgs_destructor(struct msgs *pmsgs);

/**
 * push a message to message array package
 *
 * @param[in,out] pmsgs point of message array package
 * @param[in] ppkg the message package which will be pushed to array
 *
 * @retval 0 function run success
 * @retval -1 some error occured
 */
int msgs_push(struct msgs *pmsgs, struct msgpkg *ppkg);

/**
 * pop the front message from message array package
 *
 * @param[in,out] pmsgs point of message array package
 * @param[out] ppkg the message package which will stored the poped message
 *
 * @note after called this function, the message array package will delete the
 *  first element
 *
 * @retval 0 function run success
 * @retval -1 some error occured
 */
int msgs_pop(struct msgs *pmsgs, struct msgpkg *ppkg);
#endif
