#include "GCBitArray.h"

#include <Windows.h>
#include <memory.h>

#include "../platform/GCPlatformAPI.h"
#include "GCUtils.h"

GCBitArray::GCBitArray(size_t bitCount)
{
    m_bitCount = GCUtils::AlignSize(bitCount, kUnitBit);
    size_t valCount = (m_bitCount >> 3) / sizeof(size_t);
    m_bitArray = new size_t[valCount];
    memset(m_bitArray, 0, valCount * sizeof(size_t));
}

GCBitArray::~GCBitArray()
{
    if (m_bitArray) delete[] m_bitArray;
}

void GCBitArray::setFlag(size_t offset, bool val)
{
    size_t index = offset >> kSizeTOffsetBit;
    size_t bitOffset = offset & (kUnitBit - 1);
    if (val)
        m_bitArray[index] |= (size_t(1) << bitOffset);
    else
        m_bitArray[index] &= ~(size_t(1) << bitOffset);
}

bool GCBitArray::testFlag(size_t offset) const
{
    size_t index = offset >> kSizeTOffsetBit;
    size_t bitOffset = offset & (kUnitBit - 1);
    return (m_bitArray[index] & (size_t(1) << bitOffset)) != 0;
}

size_t GCBitArray::searchNextFlag(size_t offset /*= 0*/) const
{
    size_t index = offset >> kSizeTOffsetBit;
    size_t bitOffset = offset & (kUnitBit - 1);
    if (bitOffset == 0)
    {
        int result = GCPlatformAPI::BitSearch(m_bitArray[index]);
        if (result >= 0) return index * kUnitBit + result;
        ++index;
    }
    else
    {
        size_t val = m_bitArray[index];
        for (size_t i = bitOffset; i < kUnitBit; ++i)
        {
            if ((val & (size_t(1) << i)) != 0) return index * kUnitBit + i;
        }
        ++index;
    }

    for (size_t i = index; i < getCountOfSizeT(); ++i)
    {
        int off = GCPlatformAPI::BitSearch(m_bitArray[i]);
        if (off >= 0) return i * kUnitBit + off;
    }

    return npos;
}

void GCBitArray::clear()
{
    memset(m_bitArray, 0, m_bitCount >> 3);
}

size_t GCBitArray::getCountOfSizeT() const
{
    return m_bitCount >> kSizeTOffsetBit;
}
