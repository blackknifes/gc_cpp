#include "GCBitArray.h"

#include <Windows.h>
#include <memory.h>

#include "../platform/PlatformAPI.h"

GCBitArray::GCBitArray(size_t bitCount)
{
    size_t valCount = (PlatformAPI::AlignSize(bitCount, kUnitBit) >> 3) / sizeof(size_t);
    m_bitArray = new size_t[valCount];
}

GCBitArray::~GCBitArray()
{
    if (m_bitArray) delete[] m_bitArray;
}

void GCBitArray::setFlag(size_t offset, bool val)
{
    size_t index = offset >> kUnitBit;
    size_t bitOffset = kUnitBit & (kUnitBit - 1);
    if (val)
        m_bitArray[index] |= (1 << bitOffset);
    else
        m_bitArray[index] &= ~(1 << bitOffset);
}

bool GCBitArray::isFlag(size_t offset)
{
    size_t index = offset >> 3;
    size_t bitOffset = kUnitBit & (kUnitBit - 1);
    return m_bitArray[index] &= (1 << bitOffset);
}

size_t GCBitArray::searchNextFlag(size_t offset /*= 0*/) const
{
    size_t index = offset >> 3;
    size_t bitOffset = kUnitBit & (kUnitBit - 1);
    if (bitOffset == 0)
    {
        int result = PlatformAPI::BitSearchReverse(m_bitArray[index]);

        if (result >= 0) return index * kUnitBit + result;
        ++index;
    }
    else
    {
        size_t val = m_bitArray[index];
        for (size_t i = bitOffset; i < kUnitBit; ++i)
        {
            if ((val & 1 << i) != 0) return index * kUnitBit + i;
        }
        ++index;
    }

    for (size_t i = index; i < getCountOfSizeT(); ++i)
    {
    }

    return index;
}

void GCBitArray::clear()
{
    memset(m_bitArray, 0, m_bitCount >> 3);
}

size_t GCBitArray::getCountOfSizeT() const
{
    return m_bitCount >> kOffsetBit;
}
