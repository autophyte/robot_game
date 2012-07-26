#include "robotpool.h"

/** @def SERVERADDR
 * ����server��IP��ַ
 */
#define SERVERADDR      "127.0.0.1"

/** @def SERVERPORT
 * ����server�˿�
 */
#define SERVERPORT      2208

int pool_init(robotpool *ppool) {
    int i;
    if (NULL!=ppool) {
        memset(ppool, 0, sizeof(robotpool));
        for (i=0; i<MAX_CLIENT; ++i) {
            ppool->valids[i]=1;
        }
        ppool->ids = 0;
        ppool->ncount = 0;
        return 0;
    }
    return -1;
}

/**
 * ��ȡ����һ���յģ�û��ʹ�õ�Ԫ��
 *
 * @param[in] ppool �ص�ָ��
 *
 * @retval -1 ��ȡԪ��ʧ�ܣ�����ppoolΪ�գ������ǳ�����
 */
static int pool_get_freeele(robotpool *ppool) {
    int i=-1;

    if (NULL!=ppool) {
        for (i=0; i<MAX_CLIENT; ++i) {
            if (0==ppool->valids[i]) {
                break;
            }
        }
    }

    if (MAX_CLIENT <= i) {
        i=-1;
    }

    return i;
}

int pool_create_robot(robotpool *ppool) {
    int ifree, iret;

    do {
        iret = -1;
        if (NULL==ppool) {
            break;
        }

        ifree = pool_get_freeele(ppool);
        if (0>ifree) {
            break;
        }

        iret = rob_init(&ppool->robots[ifree], ppool->ids, ifree,
            SERVERADDR, SERVERPORT);
        if (0>iret) {
            break;
        }
        ppool->valids[ifree] = 0;
    } while (0);

    return iret;
}