#include "GCFixedSpace.h"

#include "../GCConstants.h"
#include "../template/GCUtils.h"
#include "GCHeapSettings.h"

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
    auto ptr = m_ranges.find(pData);
}
