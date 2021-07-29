#ifndef __GCMEMORYSET_H__
#define __GCMEMORYSET_H__
#include <vector>

#include "GCType.h"

/**
 * 记忆集
 */
class GCMemorySet
{
public:
    virtual ~GCMemorySet();
    /**
     * 指定集合标记为脏
     * @param addr
     */
    virtual void setDirty(GCAddress addr) = 0;
    virtual void setDirty(size_t index) = 0;
    /**
     * 判断指定集合是否为脏
     * @param addr
     */
    virtual void isDirty(GCAddress addr) const = 0;
    virtual void isDirty(size_t index) const = 0;
    /*
     * 获取脏卡
     */
    virtual void getDirties(std::vector<size_t>& dirtyIndices) const = 0;
};
#endif