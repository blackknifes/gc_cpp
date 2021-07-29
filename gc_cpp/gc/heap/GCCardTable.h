#ifndef __GCCARDTABLE_H__
#define __GCCARDTABLE_H__
#include "GCMemorySet.h"

/**
 * gc卡表
 */
class GCCardTable : public GCMemorySet
{
public:
    static constexpr const size_t kDefaultCardSize = 512;  //默认单卡为512B

    GCCardTable(size_t cardSize);

    void setDirty(GCAddress addr) override;

    void setDirty(size_t index) override;

    void isDirty(GCAddress addr) const override;

    void isDirty(size_t index) const override;

    void getDirties(std::vector<size_t>& dirtyIndices) const override;

private:
    const size_t m_cardSize;  //单卡尺寸
};
#endif