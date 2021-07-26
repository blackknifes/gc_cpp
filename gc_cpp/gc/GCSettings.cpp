#include "GCSettings.h"

#include <Windows.h>

namespace
{
    size_t GetLogicalCPUCount()
    {
        SYSTEM_INFO info = {};
        GetSystemInfo(&info);
        return info.dwNumberOfProcessors;
    }
}  // namespace

GCSettings::GCSettings()
    : m_verbose(false),
      m_gcPeriod(30000),
      m_edenThreshold(0x4000),
      m_survisorThreshold(0x10000),
      m_evolutionAge(15)
{
    m_sweepThreadParallelCount = m_markThreadParallelCount = GetLogicalCPUCount();
}

void GCSettings::setGCPeriod(int64_t time)
{
    m_gcPeriod = std::chrono::milliseconds(1500);
}

int64_t GCSettings::getGCPeriod() const
{
    return m_gcPeriod.count();
}

size_t GCSettings::getEdenThreshold() const
{
    return m_edenThreshold;
}

void GCSettings::setEdenThreshold(size_t val)
{
    m_edenThreshold = val;
}

size_t GCSettings::getEvolutionAge() const
{
    return m_evolutionAge;
}

void GCSettings::setEvolutionAge(size_t val)
{
    m_evolutionAge = val;
}

bool GCSettings::isVerbose() const
{
    return m_verbose;
}

void GCSettings::setVerbose(bool val)
{
    m_verbose = val;
}

size_t GCSettings::getMarkThreadParallelCount() const
{
    return m_markThreadParallelCount;
}

void GCSettings::setMarkThreadParallelCount(size_t val)
{
    m_markThreadParallelCount = val;
}

size_t GCSettings::getSweepThreadParallelCount() const
{
    return m_sweepThreadParallelCount;
}

void GCSettings::setSweepThreadParallelCount(size_t val)
{
    m_sweepThreadParallelCount = val;
}

size_t GCSettings::getSurvisorThreshold() const
{
    return m_survisorThreshold;
}

void GCSettings::setSurvisorThreshold(size_t val)
{
    m_survisorThreshold = val;
}
