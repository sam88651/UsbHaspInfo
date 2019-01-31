/*
 * Copyright (C) 2018 Sam88651.
 * 
 * Module Name:
 *     KeyValue.c
 * Abstract:
 *     Simple key/value handler
 * Notes:
 * Revision History:
 */
#include <stdio.h>
#include <stdlib.h>
#include "hasp.h"

static int cmpfunc (const void * a, const void * b) {
   return ( ((KeyValue *)a)->key - ((KeyValue *)b)->key );
}

void initKeyValueList(KeyValue *list, int lsize) {
    
    qsort(list, lsize, sizeof(KeyValue), cmpfunc);
}

char *getValueByKey(KeyValue *list, int lsize, int key) {
    KeyValue *item;
    
    item = (KeyValue *)bsearch(&key, list, lsize, sizeof(KeyValue), cmpfunc);
    return item == NULL? "Unknown key" : item->value;
}
