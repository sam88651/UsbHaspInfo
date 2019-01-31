/*
 * Copyright (C) 2018 Sam88651.
 * 
 * Module Name:
 *     LicStatus.c
 * Abstract:
 *     Theoretically it has to show number of used HASP4 Net licenses.
 *  But, currently dues to old unsupported API it doesn't work.
 * Notes:
 * Revision History:
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hasp.h"

static int DATAFAR_ p1, DATAFAR_ p2, DATAFAR_ p3, DATAFAR_ p4;
static int activeLogins, maxLogins, keyType, activations;
static int progNum = PROG_NUM_1C;
static int seedNum = 0;

int getLicenseStatus(Hasp4 *key, const char nethaspiniPath[]) {

    /* Set nethasp.ini path for license manager handling                                    */
    p1 = 0;
    p2 = strlen(nethaspiniPath);
    p3 = 0;
    p4 = (int)nethaspiniPath;
    seedNum = rand();
    hasp(NETHASP_SET_CONFIG_FILENAME, 0, 1, 0, 0, &p1, &p2, &p3, &p4);
    if (p3) {
        return p3;
    }

    /* Login before can request license status                                              */
    p1 = p2 = p3 = p4 = 0;
    seedNum = rand();
    hasp(NETHASP_LOGIN_PROCESS, seedNum, progNum, key->pass1, key->pass2, &p1, &p2, &p3, &p4);
    if (p3) {
        return p3;
    }
    
    /* Request license manager status                                                       */
    activeLogins = maxLogins = keyType = activations = 0;
    seedNum = rand();
    hasp(NETHASP_QUERY_LICENSE, seedNum, progNum, key->pass1, key->pass2,
            &activeLogins,   /* number of currently active logins for given portNum          */
            &maxLogins,      /* maximum number of concurrent logins for given portNum        */
            &keyType,        /* maximum number of total concurrent logins the key            */
            &activations);   /* number of remaining activations (logins) for given portNum   */

    /* We have to call NETHASP_LASTSTATUS here because license data was returned in p1..p4   */
    p1 = p2 = p3 = p4 = 0;
    seedNum = rand();
    hasp(NETHASP_LASTSTATUS, seedNum, 0, 0, 0, &p1, &p2, &p3, &p4);

    if (!p3) {
        key->licensesUsed = activeLogins-1;
    }
    /*  Logout before we leave                                                              */
    hasp(NETHASP_LOGOUT, seedNum, progNum, key->pass1, key->pass2, &p1, &p2, &p3, &p4);

    return p3;
}