#include "GCPlatformAPI.h"

#include <Windows.h>
#include <assert.h>
#include <stdio.h>

#include "../GCType.h"

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

    _NT_TIB* GetTib()
    {
#ifdef _M_IX86
        return (_NT_TIB*)__readfsdword(0x18);
#elif _M_AMD64
        return (_NT_TIB*)__readgsqword(0x30);
#endif
    }

    GCAddressRange s_mainThreadAddres = {};
}  // namespace

bool GCPlatformAPI::InitMainThread()
{
    s_mainThreadAddres = GetCurrentThreadStackRange();
    return s_mainThreadAddres.begin && s_mainThreadAddres.end;
}

void GCPlatformAPI::UninitMainThread()
{
    memset(&s_mainThreadAddres, 0, sizeof(s_mainThreadAddres));
}

bool GCPlatformAPI::IsMainThread()
{
    size_t mainStack[1];
    GCAddress addr = (GCAddress)mainStack;
    return addr >= s_mainThreadAddres.begin && addr < s_mainThreadAddres.end;
}

GCAddressRange GCPlatformAPI::GetCurrentThreadStackRange()
{
    GCAddressRange range;
    _NT_TIB* tib = GetTib();
    range.begin = (GCAddress)tib->StackLimit;
    range.end = (GCAddress)tib->StackBase;
    return range;
}

void* GCPlatformAPI::MemoryAllocate(size_t _size)
{
    return VirtualAlloc(nullptr, _size, MEM_RESERVE, PAGE_READWRITE);
}

void GCPlatformAPI::MemoryFree(void* pData)
{
    VirtualFree(pData, 0, MEM_RELEASE);
}

bool GCPlatformAPI::MemoryCommit(void* pData, size_t _size)
{
    return VirtualAlloc(nullptr, _size, MEM_COMMIT, PAGE_READWRITE);
}

bool GCPlatformAPI::MemoryDecommit(void* pData, size_t _size)
{
    return VirtualFree(pData, _size, MEM_DECOMMIT);
}

int GCPlatformAPI::BitSearch(size_t val)
{
    DWORD result = 0;

#ifdef _WIN64
    if (!_BitScanForward64(&result, val)) return -1;
#else
    if (!_BitScanForward(&result, (DWORD)val)) return -1;
#endif
    return (int)result;
}

int GCPlatformAPI::BitSearchReverse(size_t val)
{
    DWORD result = 0;
#ifdef _WIN64
    if (!_BitScanReverse64(&result, val)) return -1;
#else
    if (!_BitScanReverse(&result, (DWORD)val)) return -1;
#endif
    return (int)result;
}

uint64_t GCPlatformAPI::CurrentUTFTime()
{
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    return SysTimeToUInt64(sysTime);
}

uint64_t GCPlatformAPI::UTFToLocalTime(uint64_t time)
{
    SYSTEMTIME sysTime;
    SYSTEMTIME localeTime;
    UInt64ToSysTime(time, &sysTime);
    SystemTimeToTzSpecificLocalTime(nullptr, &sysTime, &localeTime);
    return SysTimeToUInt64(localeTime);
}

uint64_t GCPlatformAPI::LocalToUTFTime(uint64_t time)
{
    SYSTEMTIME sysTime;
    SYSTEMTIME localeTime;
    UInt64ToSysTime(time, &sysTime);
    TzSpecificLocalTimeToSystemTime(nullptr, &sysTime, &localeTime);
    return SysTimeToUInt64(localeTime);
}

PlatformTime GCPlatformAPI::ConvertToPlatformTime(uint64_t time)
{
    SYSTEMTIME sysTime;
    UInt64ToSysTime(time, &sysTime);
    return reinterpret_cast<const PlatformTime&>(sysTime);
}

size_t GCPlatformAPI::GetHardwareMemorySize() 
{
    MEMORYSTATUS status = {};
    GlobalMemoryStatus(&status);
    return status.dwTotalPhys;
}

void GCPlatformAPI::FormatNormalTime(const PlatformTime& platTime, char* buf, size_t bufsize)
{
    sprintf_s(buf,
              bufsize,
              "%04u-%02u-%02u %02u:%02u:%02u.%u",
              platTime.years,
              platTime.months,
              platTime.daysOfMohth,
              platTime.hours,
              platTime.minitues,
              platTime.seconds,
              platTime.milliseconds);
}
