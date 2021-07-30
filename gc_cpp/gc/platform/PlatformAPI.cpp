#include "PlatformAPI.h"

#include <Windows.h>

void* PlatformAPI::MemoryAllocate(size_t _size)
{
    return VirtualAlloc(nullptr, _size, MEM_RESERVE, PAGE_READWRITE);
}

void PlatformAPI::MemoryFree(void* pData)
{
    VirtualFree(pData, 0, MEM_RELEASE);
}

bool PlatformAPI::MemoryCommit(void* pData, size_t _size)
{
    return VirtualAlloc(nullptr, _size, MEM_COMMIT, PAGE_READWRITE);
}

bool PlatformAPI::MemoryDecommit(void* pData, size_t _size)
{
    return VirtualFree(pData, _size, MEM_DECOMMIT);
}

size_t PlatformAPI::AlignSize(size_t _size, size_t alignSize)
{
    return ((_size + alignSize - 1) & ~(alignSize - 1));
}

int PlatformAPI::BitSearch(size_t val)
{
    DWORD result = 0;

#ifdef _WIN64
    if (!_BitScanForward64(&result, val)) return -1;
#else
    if (!_BitScanForward(&result, (DWORD)val)) return -1;
#endif
    return (int)result;
}

int PlatformAPI::BitSearchReverse(size_t val)
{
    DWORD result = 0;
#ifdef _WIN64
    if (!_BitScanReverse64(&result, val)) return -1;
#else
    if (!_BitScanReverse(&result, (DWORD)val)) return -1;
#endif
    return (int)result;
}
