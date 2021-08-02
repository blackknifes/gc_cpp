#ifndef __GCPLATFORMAPI_H__
#define __GCPLATFORMAPI_H__
#include <stdint.h>

#include "../heap/GCType.h"

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

class GCPlatformAPI
{
public:
    /**
     * ��ʼ�����߳�
     * @return
     */
    static bool InitMainThread();
    /**
     * �������̻߳���
     */
    static void UninitMainThread();

    /** �Ƿ������߳� */
    static bool IsMainThread();

    static GCAddressRange GetCurrentThreadStackRange();

    static void* MemoryAllocate(size_t _size);
    static void MemoryFree(void* pData);

    static bool MemoryCommit(void* pData, size_t _size);
    static bool MemoryDecommit(void* pData, size_t _size);

    static int BitSearch(size_t val);
    static int BitSearchReverse(size_t val);

    static uint64_t CurrentUTFTime();
    static uint64_t UTFToLocalTime(uint64_t time);
    static uint64_t LocalToUTFTime(uint64_t time);

    static PlatformTime ConvertToPlatformTime(uint64_t time);

    static size_t GetHardwareMemorySize();

    /**
     * ��ʽ��Ϊ yyyy-MM-dd hh:mm:ss.ms ��׼ʱ���ʽ
     */
    static void FormatNormalTime(const PlatformTime& platTime, char* buf, size_t bufsize);
};
#endif