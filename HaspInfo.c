/*
 * Copyright (C) 2018 Sam88651.
 * 
 * Module Name:
 *     HaspInfo.c
 * Abstract:
 *     Utility to find and check USB HASP4 keys attached to the system
 * Notes:
 * Revision History:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <linux/limits.h>
#include <unistd.h>
#include "hasp.h"

/*
 * Plug to support old libhasplnx.a
 */
int __ctype_toupper(int c) {
    return toupper(c);
}

/* Found keys descriptions  */
static Hasp4 keys [MAX_1C_KEYS];
/* Number of found keys     */
static int nKeys = sizeof(keys)/sizeof(keys[0]);
static char nethaspiniPath [PATH_MAX] = "/etc/haspd/nethasp.ini";


int main(int argc, char *argv[]) {
    register int i, r;
    int     opt;

    while ((opt=getopt(argc,argv, "?hc:")) != -1) {
        switch (opt) {
        case 'c':
            strncpy(nethaspiniPath, (char *)optarg, sizeof(nethaspiniPath));
            nethaspiniPath[sizeof(nethaspiniPath)-1] = '\0';
            break;
        default:
        case '?':
        case 'h':
            fprintf (stderr,"Usage: #%s [-c] <nethasp.ini path>\n", argv[0]);
            return -1;
        }
    }
    
    r = enumKeys(keys, &nKeys, nethaspiniPath);
    if (!r) {
        if (!nKeys) {
            r = HASPERR_HASP_NOT_FOUND;
            printHaspErrorMessage(r);
        } else {
            printf("HASP OK: ");
            for (i = 0; i < nKeys; i++) {
                printf ("%s (%ld %s %d", getHaspType(keys[i].type), 
                        keys[i].id, keys[i].licensesTotal==1?"local":"network",
                        keys[i].licensesTotal);
                if (keys[i].licensesTotal > 1) {
                    printf(" %d", keys[i].licensesUsed);
                }
                if (i+1 != nKeys)
                    printf("), ");
                else
                    printf(") | ");
            }            
            // 'label'=value[UOM];[warn];[crit];[min];[max]
            for (i = 0; i < nKeys; i++) {
                if (keys[i].licensesTotal > 1) {
                    printf ("SN_%ld=%d;%d;%d ", keys[i].id, 
                        keys[i].licensesUsed,
                        (keys[i].licensesTotal*7)/10,
                        (keys[i].licensesTotal*9)/10);
                } else {
                    printf ("SN_%ld=1;1;1 ", keys[i].id);                    
                }
            }
            printf("\n");
        }
    } else {
        printHaspErrorMessage(r);
    }
    return(!r?NAGIOS_OK:NAGIOS_CRITICAL);
}

