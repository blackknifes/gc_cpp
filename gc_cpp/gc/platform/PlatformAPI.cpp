#include "PlatformAPI.h"

#include <Windows.h>

namespace
{
    uint64_t FileTimeToUInt64(const FILETIME& fileTime)
    {
        ULARGE_INTEGER ul;
        ul.LowPart = fileTime.dwLowDateTime;
        ul.HighPart = fileTime.dwHighDateTime;
        return ul.QuadPart;
    }

    uint64_t SysTimeToUInt64(const SYSTEMTIME& sysTime)
    {
        FILETIME fl;
        SystemTimeToFileTime(&sysTime, &fl);
        return FileTimeToUInt64(fl);
    }

    void UInt64ToFileTime(uint64_t time, LPFILETIME fileTime)
    {
        ULARGE_INTEGER ul;
        ul.QuadPart = time;
        fileTime->dwLowDateTime = ul.LowPart;
        fileTime->dwHighDateTime = ul.HighPart;
    }

    void UInt64ToSysTime(uint64_t time, LPSYSTEMTIME sysTime)
    {
        FILETIME fileTime;
        UInt64ToFileTime(time, &fileTime);
        FileTimeToSystemTime(&fileTime, sysTime);
    }

}  // namespace

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

uint64_t PlatformAPI::CurrentUTFTime()
{
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    return SysTimeToUInt64(sysTime);
}

uint64_t PlatformAPI::UTFToLocalTime(uint64_t time)
{
    SYSTEMTIME sysTime;
    SYSTEMTIME localeTime;
    UInt64ToSysTime(time, &sysTime);
    SystemTimeToTzSpecificLocalTime(nullptr, &sysTime, &localeTime);
    return SysTimeToUInt64(localeTime);
}

uint64_t PlatformAPI::LocalToUTFTime(uint64_t time)
{
    SYSTEMTIME sysTime;
    SYSTEMTIME localeTime;
    UInt64ToSysTime(time, &sysTime);
    TzSpecificLocalTimeToSystemTime(nullptr, &sysTime, &localeTime);
    return SysTimeToUInt64(localeTime);
}

PlatformTime PlatformAPI::ConvertToPlatformTime(uint64_t time)
{
    SYSTEMTIME sysTime;
    UInt64ToSysTime(time, &sysTime);
    return reinterpret_cast<const PlatformTime&>(sysTime);
}
