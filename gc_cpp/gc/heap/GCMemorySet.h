#ifndef __GCMEMORYSET_H__
#define __GCMEMORYSET_H__
#include <vector>

#include "GCType.h"

/**
 * ���伯
 */
class GCMemorySet
{
public:
    virtual ~GCMemorySet();
    /**
     * ָ�����ϱ��Ϊ��
     * @param addr
     */
    virtual void setDirty(GCAddress addr) = 0;
    virtual void setDirty(size_t index) = 0;
    /**
     * �ж�ָ�������Ƿ�Ϊ��
     * @param addr
     */
    virtual void isDirty(GCAddress addr) const = 0;
    virtual void isDirty(size_t index) const = 0;
    /*
     * ��ȡ�࿨
     */
    virtual void getDirties(std::vector<size_t>& dirtyIndices) const = 0;
};
#endif