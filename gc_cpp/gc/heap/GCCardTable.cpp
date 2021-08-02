#include "GCCardTable.h"

#include "../platform/GCPlatformAPI.h"
#include "../template/GCUtils.h"

GCCardTable::GCCardTable(GCAddress startAddr, size_t numOfCards)
    : m_startAddr(startAddr),
      m_bitArray(numOfCards)
{
}

void GCCardTable::setDirty(GCAddress addr)
{
    size_t index = addr - m_startAddr;
    index = GCUtils::AlignSize(index, kCardSize) >> 9;
    setDirty(index);
}

void GCCardTable::setDirty(size_t index)
{
    m_bitArray.setFlag(index, true);
}

bool GCCardTable::isDirty(GCAddress addr) const
{
    size_t index = addr - m_startAddr;
    index = GCUtils::AlignSize(index, kCardSize) >> 9;
    return isDirty(index);
}

bool GCCardTable::isDirty(size_t index) const
{
    return m_bitArray.testFlag(index);
}

size_t GCCardTable::findDirty(GCAddress addr) const
{
    size_t index = addr - m_startAddr;
    index = GCUtils::AlignSize(index, kCardSize) >> 9;
    return findDirty(index);
}

size_t GCCardTable::findDirty(size_t startOff) const
{
    return m_bitArray.searchNextFlag(startOff);
}

size_t GCCardTable::getDirtyBlockSize() const
{
    return kCardSize;
}