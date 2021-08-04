#ifndef __GCTYPE_H__
#define __GCTYPE_H__
#include <stdint.h>

typedef uint8_t* GCAddress;

struct GCAddressRange
{
    GCAddress begin;
    GCAddress end;
};

enum GCMarkColor : uint32_t
{
    GC_MARK_WHITE = 0,
    GC_MARK_BLACK,
    GC_MARK_GRAY
};

struct GCMarkFlag
{
    GCMarkColor color : 2;              //��ɫ���
    uint32_t dirty : 1;                 //�Ƿ�Ϊ��
    uint32_t age : sizeof(size_t) - 2;  //����
};

enum GCGeneration
{
    GC_GENERATION_EDEN,
    GC_GENERATION_SURVIVOR,
    GC_GENERATION_OLD
};
#endif