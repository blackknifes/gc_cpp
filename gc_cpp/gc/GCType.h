#ifndef __GCTYPE_H__
#define __GCTYPE_H__
#include <stdint.h>

typedef uint8_t* GCAddress;

struct GCAddressRange
{
    GCAddress begin;
    GCAddress end;
};
#endif