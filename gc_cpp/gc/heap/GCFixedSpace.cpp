#include "GCFixedSpace.h"

#include "../template/GCUtils.h"

GCFixedSpace::GCFixedSpace(const size_t fixedSize)
    : m_fixedSize(fixedSize),
      m_ranges(),
      m_entry(nullptr)
{
}

void* GCFixedSpace::alloc(size_t _size)
{
    return nullptr;
}

void GCFixedSpace::dealloc(void* pData)
{
    size_t addrSize = (size_t)pData;
    GCUtils::AlignSize(addrSize, );
}
