#ifndef __GCCARDTABLE_H__
#define __GCCARDTABLE_H__
#include "../template/GCBitArray.h"
#include "GCMemorySet.h"

/**
 * gc卡表
 */
class GCCardTable : public GCMemorySet
{
public:
    static constexpr const size_t kCardSize = 512;  //默认单卡为512B

    GCCardTable(GCAddress startAddr, size_t numOfCards);

    void setDirty(GCAddress addr) override;
    void setDirty(size_t index) override;
    bool isDirty(GCAddress addr) const override;
    bool isDirty(size_t index) const override;
    size_t findDirty(GCAddress addr) const override;
    size_t findDirty(size_t startOff) const override;
    size_t getDirtyBlockSize() const override;

private:
    GCBitArray m_bitArray;
    GCAddress m_startAddr;
};
#endif