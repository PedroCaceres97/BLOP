#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef __BLOP_BASICS_H__
#define __BLOP_BASICS_H__

#define BLOP_CONCAT2_IMPL(a, b) a##b
#define BLOP_CONCAT3_IMPL(a, b, c) a##b##c

#define BLOP_CONCAT2(a, b) BLOP_CONCAT2_IMPL(a, b)
#define BLOP_CONCAT3(a, b, c) BLOP_CONCAT3_IMPL(a, b, c)

#endif // __BLOP_BASICS_H__

#ifndef BLOP_VECTOR_NAME
    #define BLOP_VECTOR_NAME blop
#endif

#ifndef BLOP_VECTOR_DATA_TYPE
    #define BLOP_VECTOR_DATA_TYPE uint8_t
#endif

#if !defined(BLOP_VECTOR_SCALATOR) || BLOP_VECTOR_SCALATOR < 2
    #define BLOP_VECTOR_SCALATOR 2
#endif

#ifndef BLOP_VECTOR_DEALLOCATE_DATA
    #define BLOP_VECTOR_DEALLOCATE_DATA(ptr)
#endif

#ifdef BLOP_VECTOR_SAFE_MODE
    #define BLOP_VECTOR_ASSERT_PTR(ptr, rt) if (ptr == NULL) {printf("[BLOP -> vector.h]: " #ptr " is a null ptr (returning without effect)"); BLOP_LIST_EXIT; return rt;}
    #define BLOP_VECTOR_ASSERT_PTR_VOID(ptr) if (ptr == NULL) {printf("[BLOP -> vector.h]: " #ptr " is a null ptr (returning without effect)"); BLOP_LIST_EXIT; return;}
#else
    #define BLOP_VECTOR_ASSERT_PTR(ptr, rt)
    #define BLOP_VECTOR_ASSERT_PTR_VOID(ptr)
#endif

#ifdef BLOP_VECTOR_EXIT_ON_ERROR
    #define BLOP_VECTOR_EXIT exit(-1)
#else
    #define BLOP_VECTOR_EXIT
#endif

#ifndef BLOP_VECTOR_CAMEL

    #define _BLOPS_VECTOR              BLOP_CONCAT3(_, BLOP_VECTOR_NAME, _t)
    #define _BLOPT_VECTOR              BLOP_VECTOR_NAME

    #define _BLOPF_VECTOR_CREATE       BLOP_CONCAT2(BLOP_VECTOR_NAME, _create)
    #define _BLOPF_VECTOR_DESTROY      BLOP_CONCAT2(BLOP_VECTOR_NAME, _destroy)

    #define _BLOPF_VECTOR_SET          BLOP_CONCAT2(BLOP_VECTOR_NAME, _set)
    #define _BLOPF_VECTOR_GET          BLOP_CONCAT2(BLOP_VECTOR_NAME, _get)
    #define _BLOPF_VECTOR_GET_SIZE     BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_data)
    #define _BLOPF_VECTOR_GET_DATA     BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_size)
    #define _BLOPF_VECTOR_GET_BACK     BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_back)
    #define _BLOPF_VECTOR_GET_FRONT    BLOP_CONCAT2(BLOP_VECTOR_NAME, _get_front)

    #define _BLOPF_VECTOR_CLEAR        BLOP_CONCAT2(BLOP_VECTOR_NAME, _clear)
    #define _BLOPF_VECTOR_ERASE        BLOP_CONCAT2(BLOP_VECTOR_NAME, _erase)
    #define _BLOPF_VECTOR_INSERT       BLOP_CONCAT2(BLOP_VECTOR_NAME, _insert)
    #define _BLOPF_VECTOR_POP_BACK     BLOP_CONCAT2(BLOP_VECTOR_NAME, _pop_back)
    #define _BLOPF_VECTOR_POP_FRONT    BLOP_CONCAT2(BLOP_VECTOR_NAME, _pop_front)
    #define _BLOPF_VECTOR_PUSH_BACK    BLOP_CONCAT2(BLOP_VECTOR_NAME, _push_back)
    #define _BLOPF_VECTOR_PUSH_FRONT   BLOP_CONCAT2(BLOP_VECTOR_NAME, _push_front)

#else // BLOP_VECTOR_CAMEL

    #define _BLOPS_VECTOR              BLOP_CONCAT3(_, BLOP_VECTOR_NAME, _t)
    #define _BLOPT_VECTOR              BLOP_VECTOR_NAME

    #define _BLOPF_VECTOR_CREATE       BLOP_CONCAT2(BLOP_VECTOR_NAME, Create)
    #define _BLOPF_VECTOR_DESTROY      BLOP_CONCAT2(BLOP_VECTOR_NAME, Destroy)

    #define _BLOPF_VECTOR_SET          BLOP_CONCAT2(BLOP_VECTOR_NAME, Set)
    #define _BLOPF_VECTOR_GET          BLOP_CONCAT2(BLOP_VECTOR_NAME, Get)
    #define _BLOPF_VECTOR_GET_SIZE     BLOP_CONCAT2(BLOP_VECTOR_NAME, GetData)
    #define _BLOPF_VECTOR_GET_DATA     BLOP_CONCAT2(BLOP_VECTOR_NAME, GetSize)
    #define _BLOPF_VECTOR_GET_BACK     BLOP_CONCAT2(BLOP_VECTOR_NAME, GetBack)
    #define _BLOPF_VECTOR_GET_FRONT    BLOP_CONCAT2(BLOP_VECTOR_NAME, GetFront)

    #define _BLOPF_VECTOR_CLEAR        BLOP_CONCAT2(BLOP_VECTOR_NAME, Clear)
    #define _BLOPF_VECTOR_ERASE        BLOP_CONCAT2(BLOP_VECTOR_NAME, Erase)
    #define _BLOPF_VECTOR_INSERT       BLOP_CONCAT2(BLOP_VECTOR_NAME, Insert)
    #define _BLOPF_VECTOR_POP_BACK     BLOP_CONCAT2(BLOP_VECTOR_NAME, PopBack)
    #define _BLOPF_VECTOR_POP_FRONT    BLOP_CONCAT2(BLOP_VECTOR_NAME, PopFront)
    #define _BLOPF_VECTOR_PUSH_BACK    BLOP_CONCAT2(BLOP_VECTOR_NAME, PushBack)
    #define _BLOPF_VECTOR_PUSH_FRONT   BLOP_CONCAT2(BLOP_VECTOR_NAME, PushFront)

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BLOPS_VECTOR* _BLOPT_VECTOR;

_BLOPT_VECTOR           _BLOPF_VECTOR_CREATE(size_t inits);

void                    _BLOPF_VECTOR_DESTROY(_BLOPT_VECTOR vec);

void                    _BLOPF_VECTOR_SET(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);       
BLOP_VECTOR_DATA_TYPE   _BLOPF_VECTOR_GET(_BLOPT_VECTOR vec, size_t idx);       
size_t                  _BLOPF_VECTOR_GET_SIZE(_BLOPT_VECTOR vec);  
uint8_t*                _BLOPF_VECTOR_GET_DATA(_BLOPT_VECTOR vec);  
BLOP_VECTOR_DATA_TYPE   _BLOPF_VECTOR_GET_BACK(_BLOPT_VECTOR vec);  
BLOP_VECTOR_DATA_TYPE   _BLOPF_VECTOR_GET_FRONT(_BLOPT_VECTOR vec); 
void                    _BLOPF_VECTOR_CLEAR(_BLOPT_VECTOR vec);     
void                    _BLOPF_VECTOR_ERASE(_BLOPT_VECTOR vec);     
void                    _BLOPF_VECTOR_INSERT(_BLOPT_VECTOR vec, size_t idx, BLOP_VECTOR_DATA_TYPE value);    
void                    _BLOPF_VECTOR_POP_BACK(_BLOPT_VECTOR vec);  
void                    _BLOPF_VECTOR_POP_FRONT(_BLOPT_VECTOR vec); 
void                    _BLOPF_VECTOR_PUSH_BACK(_BLOPT_VECTOR vec, BLOP_VECTOR_DATA_TYPE value); 
void                    _BLOPF_VECTOR_PUSH_FRONT(_BLOPT_VECTOR vec, BLOP_VECTOR_DATA_TYPE value);

#ifdef BLOP_VECTOR_IMPLEMENTATION

#endif

#ifdef __cplusplus
}
#endif

#undef BLOP_LIST_SAFE_MODE
#undef BLOP_LIST_EXIT_ON_ERROR
#undef BLOP_LIST_ASSERT_PTR
#undef BLOP_LIST_EXIT

#undef BLOP_LIST_NAME
#undef BLOP_LIST_DATA_TYPE
#undef BLOP_LIST_DEALLOCATE_DATA