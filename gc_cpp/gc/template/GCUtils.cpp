#include "GCUtils.h"

size_t GCUtils::AlignSize(size_t _size, size_t alignSize) 
{
    return ((_size + alignSize - 1) & ~(alignSize - 1));
}
