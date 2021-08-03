#ifndef __GCUTILS_H__
#define __GCUTILS_H__
#include <xutility>

#include "../GCType.h"
#include "../heap/GCMemoryHeader.h"

class GCUtils
{
public:
    template<typename _Ty, typename... _argsTy>
    static void Construct(_Ty* ptr, _argsTy&&... args)
    {
        new (ptr) _Ty(std::move(args)...);
    }
    template<typename _Ty>
    static void Destroy(_Ty* ptr)
    {
        ptr->~_Ty();
    }

    static size_t AlignSize(size_t _size, size_t alignSize);
    static GCMemoryHeader* GetMemoryHeader(GCAddress pData);
    static GCAddress GetMemoryData(GCMemoryHeader* pHeader);
};
#endif