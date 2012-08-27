#include "msgs.h"
#include <string.h>
#include <stdlib.h>

/* construct a msgs array package */
void msgs_constructor(struct msgs *pmsgs, _uint count) {
    if (pmsgs && count) {
        pmsgs->max_slots =   count;
        pmsgs->front     =   0;
        pmsgs->rear      =   0;
        pmsgs->buf       =   (struct msgpkg *)calloc(count, sizeof(struct msgpkg));
        sem_init(&pmsgs->mutex, 0, 1);
        sem_init(&pmsgs->slots, 0, count);
        sem_init(&pmsgs->items, 0, 0);

        if (pmsgs->buf) {
            msgpkg_constructor_arr(pmsgs->buf, count);
        }
    }
}

/* destruct a msgs array package */
void msgs_destructor(struct msgs *pmsgs) {
    if (pmsgs && pmsgs->buf) {
        free(pmsgs->buf);
        pmsgs->buf = NULL;
    }
}

/* push a message to message array package */
int msgs_push(struct msgs *pmsgs, struct msgpkg *ppkg) {
    if (pmsgs && ppkg) {
        sem_wait(&pmsgs->slots);
        sem_wait(&pmsgs->mutex);
        msgpkg_copy(&(pmsgs->buf[(++pmsgs->rear)%(pmsgs->max_slots)]), ppkg);
        sem_post(&pmsgs->mutex);
        sem_post(&pmsgs->items);

        return 0;
    }
    return -1;
}

/* pop the front message from message array package */
int msgs_pop(struct msgs *pmsgs, struct msgpkg *ppkg) {
    if (pmsgs && ppkg) {
        sem_wait(&pmsgs->items);
        sem_wait(&pmsgs->mutex);
        msgpkg_copy(ppkg, &(pmsgs->buf[(++pmsgs->front)%(pmsgs->max_slots)]));
        sem_post(&pmsgs->mutex);
        sem_post(&pmsgs->slots);

        return 0;
    }
    return -1;
}
