#include "GCHeap.h"

#include <assert.h>

namespace
{
    GCHeap* s_heap = nullptr;
}

GCHeap* GCHeap::GetHeap()
{
    return s_heap;
}

GCHeap::GCHeap()
{
    assert(s_heap == nullptr);
    s_heap = this;
}

GCHeap::~GCHeap()
{
    assert(s_heap == this);
    s_heap = nullptr;
}

GCAddress GCHeap::alloc()
{
    return nullptr;
}

void GCHeap::dealloc(GCAddress addr) {}

const GCHeapSettings& GCHeap::getSettings() const
{
    return m_settings;
}
