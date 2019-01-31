/*
 * Copyright (C) 2018 Sam88651.
 * 
 * Module Name:
 *     EnumKeys.c
 * Abstract:
 *     Check usb hasp4 keys availability via old Aladdin HASP4 API.
 * Notes:
 * Revision History:
 */
#include <stdio.h>
#include <stdlib.h>
#include "hasp.h"

/* List of passwords to check   */
static int passwords [3] [2] = {
    { 0x3B6D, 0x70CB },
    { 0x4125, 0x237A },
    { 0x4E31, 0x10E4 }
};

/* API parameters can't be in stack */
static int DATAFAR_ p1, DATAFAR_ p2, DATAFAR_ p3, DATAFAR_ p4;
static int progNum = 0;
static int seedNum = 0;

/**
 * 
 * @param keys - buffer for keys
 * @param maxKeys - in - max buffer size, out - number of keys found
 * @return - error code
 */
int enumKeys (Hasp4 keys[], int *maxKeys, const char nethaspiniPath[]) {
    register int i;
    int nKeys;

    for (nKeys = 0, i = 0; i < sizeof(passwords)/(sizeof(passwords[0])) && nKeys < *maxKeys; i++) {
        p1 = p2 = p3 = p4 = 0;
        seedNum = rand();
        hasp(LOCALHASP_HASPSTATUS, seedNum, progNum, passwords[i][0], passwords[i][1], &p1, &p2, &p3, &p4);
        if (p3 != 0) {
            switch (p2) {
            case 0:
                keys [nKeys].type = HASP4_STD;
                break;
            case 1:
                switch(p1) {
                case 1: 
                    keys [nKeys].type = HASP4_M1;
                    break;
                case 4: 
                    keys [nKeys].type = HASP4_M4;
                    break;
                default:
                    keys [nKeys].type = UNKNOWN_TYPE;
                    break;
                }
                break;
            case 5:
                keys [nKeys].type = HASP4_TIME;
                break;
            default:
                keys [nKeys].type = UNKNOWN_TYPE;
                break;
            }
        } else {
            continue;
        }
        p1 = p2 = p3 = p4 = 0;
        seedNum = rand();
        hasp(LOCALHASP_HASPGENERATION, seedNum, progNum, passwords[i][0], passwords[i][1], &p1, &p2, &p3, &p4);
        if (p3) {           // error
            return p3;
        }
        if (p1 != 1)        // not HASP4
            continue;

        keys [nKeys].pass1 = passwords[i][0];
        keys [nKeys].pass2 = passwords[i][1];

        p1 = p2 = p3 = p4 = 0;
        seedNum = rand();
        hasp(LOCALHASP_HASPNETSTATUS, seedNum, progNum, passwords[i][0], passwords[i][1], &p1, &p2, &p3, &p4);
        if (p3) {
            return p3;
        } else  {
            keys [nKeys].licensesTotal = !p1?1:p1;
            if (keys[nKeys].licensesTotal > 1) {
                p3 = getLicenseStatus(&keys[nKeys], nethaspiniPath);
                if (p3) {
                    return p3;
                }
            } else {
                keys[nKeys].licensesUsed = 1;
            }
        }

        p1 = p2 = p3 = p4 = 0;
        seedNum = rand();
        hasp(MEMOHASP_HASPID, seedNum, progNum, passwords[i][0], passwords[i][1], &p1, &p2, &p3, &p4);
        if (p3) {
            return p3;
        } else {
            keys [nKeys].id = p2;
            keys [nKeys].id <<= 16;
            keys [nKeys].id |= (unsigned) p1;
        }
        ++nKeys;
        }
    *maxKeys = nKeys;
    return 0;
}
