/*msgs.h*/
#ifndef _MSGS_H_
#define _MSGS_H_ 1

#include <semaphore.h>
#include "msgpkg.h"

struct msgs {
    struct msgpkg   *buf;
    _uint           max_slots;
    _uint           front;      /**< last data is: buf[(++front)%max_slots] */
    _uint           rear;       /**< first data is: buf[rear%max_slots] */
    sem_t           mutex;      /**< 互斥锁 */
    sem_t           slots;      /**< 可用的槽位数量 */
    sem_t           items;      /**< 数据的数量 */
};

void msgs_constructor(struct msgs *pmsgs, _uint count);
void msgs_destructor(struct msgs *pmsgs);
int msgs_push(struct msgs *pmsgs, struct msgpkg *ppkg);
int msgs_pop(struct msgs *pmsgs, struct msgpkg *ppkg);
#endif
