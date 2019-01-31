/*
 * Copyright (C) 2018 Sam88651.
 * 
 * Module Name:
 *     HaspType.c
 * Abstract:
 *     Converts HASP type to string
 * Notes:
 * Revision History:
 */
#include <stdio.h>
#include <stdlib.h>
#include "hasp.h"


static KeyValue haspTypes[5] = {
    { HASP4_STD,    "HASP4 Std" },
    { HASP4_M1,     "HASP4 M1"  },
    { HASP4_M4,     "HASP4 M4"  },
    { HASP4_TIME,   "HASP4 Time" },
    { UNKNOWN_TYPE, "UNKNOWN" }
};
static int init = 0;

/**
 * HASP type to string
 * @param type - Hasp type
 * @return - type string
 */
char *getHaspType(int type) {
    
    if (!init) {
        initKeyValueList(haspTypes, sizeof(haspTypes)/sizeof(KeyValue));
        init = 1;
    }
    return getValueByKey(haspTypes, sizeof(haspTypes)/sizeof(KeyValue), type);
}
