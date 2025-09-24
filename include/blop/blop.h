#ifndef __BLOP_H__
#define __BLOP_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _BlopPool_t* BlopPool;
typedef struct _BlopApplication_t* BlopApplication;

typedef enum {
    BlopSuccess             ,      
    BlopNullException       , 
    BlopAllocationFailed    , 
    BlopDeallocationlFailed , 
    BlopIndexException      , 
    BlopWrongSignature      ,
    BlopNonEmptyStructure   ,
} BlopError;

#ifdef __BLOP_SHOW_APPLICATION_IMPLEMENTATION__

#define __BLOP_SHOW_LIST_IMPLEMENTATION__
#include <blop/list.h>

struct _BlopApplication_t {
    BlopError error;
    struct _BlopList_t pools;
    struct _BlopList_t debug_pools;
};

#endif // __BLOP_SHOW_APPLICATION_IMPLEMENTATION__

#include <blop/list.h>

BlopApplication BlopCreateApplication(int exit_on_error);
int             BlopDestroyApplication(BlopApplication app);

BlopError       BlopGetLastError(BlopApplication app);
const char*     BlopGetErrorString(BlopError error);

BlopList        BlopGetActivePools(BlopApplication app);
BlopList        BlopGetActiveDebugPools(BlopApplication app);

#endif // __BLOP_H__