#include "GCUtils.h"

size_t GCUtils::AlignSize(size_t _size, size_t alignSize)
{
    return ((_size + alignSize - 1) & ~(alignSize - 1));
}

GCMemoryHeader* GCUtils::GetMemoryHeader(GCAddress pData)
{
    return reinterpret_cast<GCMemoryHeader*>(pData - sizeof(GCMemoryHeader));
}

GCAddress GCUtils::GetMemoryData(GCMemoryHeader* pHeader)
{
    return reinterpret_cast<GCAddress>((GCAddress)pHeader + sizeof(GCMemoryHeader));
}
