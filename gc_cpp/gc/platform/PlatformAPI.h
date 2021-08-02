#ifndef __PLATFORMAPI_H__
#define __PLATFORMAPI_H__
#include <stdint.h>

struct PlatformTime
{
    uint16_t years;
    uint16_t months;
    uint16_t daysOfWeek;
    uint16_t daysOfMohth;

    uint16_t hours;
    uint16_t minitues;
    uint16_t seconds;
    uint16_t milliseconds;
};

class PlatformAPI
{
public:
    static void* MemoryAllocate(size_t _size);
    static void MemoryFree(void* pData);

    static bool MemoryCommit(void* pData, size_t _size);
    static bool MemoryDecommit(void* pData, size_t _size);

    static size_t AlignSize(size_t _size, size_t alignSize);

    static int BitSearch(size_t val);
    static int BitSearchReverse(size_t val);

    static uint64_t CurrentUTFTime();
    static uint64_t UTFToLocalTime(uint64_t time);
    static uint64_t LocalToUTFTime(uint64_t time);

    static PlatformTime ConvertToPlatformTime(uint64_t time);

    /** 
     * 格式化为 yyyy-MM-dd hh:mm:ss.ms 标准时间格式
     */
    static void FormatNormalTime(const PlatformTime& platTime, char* buf, size_t bufsize);
};
#endif