#ifndef __PLATFORMAPI_H__
#define __PLATFORMAPI_H__

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
};
#endif