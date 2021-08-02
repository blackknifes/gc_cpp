#include "GCHeapSettings.h"

#include "../platform/GCPlatformAPI.h"

GCHeapSettings::GCHeapSettings()
{
    size_t size = GCPlatformAPI::GetHardwareMemorySize();
    if (size >= 2 * 1024 * 1024 * 1024)
    {
        m_globalMaxSize = size / 2;
        m_globalMinSize = 64 * 1024 * 1024;
    }
}

size_t GCHeapSettings::getGlobalMaxSize() const
{
    return m_globalMaxSize;
}

void GCHeapSettings::setGlobalMaxSize(size_t globalMaxSize)
{
    m_globalMaxSize = globalMaxSize;
}

size_t GCHeapSettings::getGlobalMinSize() const
{
    return m_globalMinSize;
}

void GCHeapSettings::setGlobalMinSize(size_t globalMinSize)
{
    m_globalMinSize = globalMinSize;
}

size_t GCHeapSettings::getGlobalAlignSize() const
{
    return m_globalAlignSize;
}

void GCHeapSettings::setGlobalAlignSize(size_t globalAlignSize)
{
    m_globalAlignSize = globalAlignSize;
}

size_t GCHeapSettings::getCardSize() const
{
    return m_cardSize;
}

void GCHeapSettings::setCardSize(size_t cardSize)
{
    m_cardSize = cardSize;
}

size_t GCHeapSettings::getEdenSize() const
{
    return m_edenSize;
}

void GCHeapSettings::setEdenSize(size_t edenSize)
{
    m_edenSize = edenSize;
}

size_t GCHeapSettings::getThreadStackSize() const
{
    return m_threadStackSize;
}

void GCHeapSettings::setThreadStackSize(size_t threadStackSize)
{
    m_threadStackSize = threadStackSize;
}

size_t GCHeapSettings::getThreadHeapAlignSize() const
{
    return m_threadHeapAlignSize;
}

void GCHeapSettings::setThreadHeapAlignSize(size_t threadHeapAlignSize)
{
    m_threadHeapAlignSize = threadHeapAlignSize;
}
