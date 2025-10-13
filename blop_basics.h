#ifndef __BLOP_BASICS_H__
#define __BLOP_BASICS_H__

#define BLOP_CONCAT2_IMPL(a, b) a##b
#define BLOP_CONCAT3_IMPL(a, b, c) a##b##c

#define BLOP_CONCAT2(a, b) BLOP_CONCAT2_IMPL(a, b)
#define BLOP_CONCAT3(a, b, c) BLOP_CONCAT3_IMPL(a, b, c)

#endif // __BLOP_BASICS_H__