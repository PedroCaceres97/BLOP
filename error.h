#ifndef __BLOP_H__
#define __BLOP_H__

#include <blop/common.h>

typedef enum {
    BlopSuccess             ,      
    BlopNullException       , 
    BlopAllocationFailed    , 
    BlopDeallocationlFailed , 
    BlopIndexException      , 
    BlopWrongSignature      ,
    BlopNonEmptyStructure   ,
} BERROR;

void        BERROR_SetLast(BERROR error);
BERROR      BERROR_GetLast();
const char* BERROR_ToString(BERROR error);

#endif // __BLOP_H__