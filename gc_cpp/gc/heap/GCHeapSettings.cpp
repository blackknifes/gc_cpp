#include "GCHeapSettings.h"

#include "../platform/GCPlatformAPI.h"
#include "../template/GCUtils.h"
#include "GCHeap.h"

namespace
{
    size_t GetBinaryPower(size_t _size)
    {
        size_t dstSize = 128;
        while (dstSize < _size) dstSize = dstSize << 1;
        return dstSize;
    }
}  // namespace

const GCHeapSettings& GCHeapSettings::GetSettings()
{
    return GCHeap::GetHeap()->getSettings();
}

GCHeapSettings::GCHeapSettings()
{
    size_t size = GCPlatformAPI::GetHardwareMemorySize();
    if (size >= 2 * GB)
    {
        m_globalMaxSize = 2 * GB;
        m_globalMinSize = 32 * MB;
    }
    else
    {
        m_globalMaxSize = GetBinaryPower(size / 2);
        m_globalMinSize = 32 * MB;
    }
}

size_t GCHeapSettings::getGlobalMaxSize() const
{
    return m_globalMaxSize;
}

void GCHeapSettings::setGlobalMaxSize(size_t globalMaxSize)
{
    m_globalMaxSize = GetBinaryPower(globalMaxSize);
}

size_t GCHeapSettings::getGlobalMinSize() const
{
    return m_globalMinSize;
}

void GCHeapSettings::setGlobalMinSize(size_t globalMinSize)
{
    m_globalMinSize = GetBinaryPower(globalMinSize);
}

size_t GCHeapSettings::getHeapAlignSize() const
{
    return m_heapAlignSize;
}

void GCHeapSettings::setHeapAlignSize(size_t globalAlignSize)
{
    m_heapAlignSize = GetBinaryPower(globalAlignSize);
}

size_t GCHeapSettings::getCardSize() const
{
    return m_cardSize;
}

void GCHeapSettings::setCardSize(size_t cardSize)
{
    m_cardSize = GetBinaryPower(cardSize);
}

size_t GCHeapSettings::getEdenSize() const
{
    return m_edenSize;
}

void GCHeapSettings::setEdenSize(size_t edenSize)
{
    m_edenSize = GetBinaryPower(edenSize);
}

size_t GCHeapSettings::getThreadStackSize() const
{
    return m_threadStackSize;
}

void GCHeapSettings::setThreadStackSize(size_t threadStackSize)
{
    m_threadStackSize = threadStackSize;
}

size_t GCHeapSettings::getHeapSliceSize() const
{
    return m_heapSliceSize;
}

void GCHeapSettings::setHeapSliceSize(size_t threadSliceSize)
{
    m_heapSliceSize = GetBinaryPower(threadSliceSize);
}
