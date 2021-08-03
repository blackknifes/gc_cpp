#ifndef __GCCONSTANTS_H__
#define __GCCONSTANTS_H__

class GCConstants
{
public:
    static constexpr const size_t KB = 1024;
    static constexpr const size_t MB = 1024 * KB;
    static constexpr const size_t GB = 1024 * MB;
    static constexpr const size_t TB = 1024 * GB;
    static constexpr const size_t PB = 1024 * TB;
    static constexpr const size_t EB = 1024 * PB;
    static constexpr const size_t MemoryGranularity = 8;
};
#endif