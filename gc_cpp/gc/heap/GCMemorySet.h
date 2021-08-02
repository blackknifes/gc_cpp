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
    static constexpr const size_t npos = size_t(-1);

    virtual ~GCMemorySet();

    /**
     * ��ȡ��λ���Ĵ�С
     * @return
     */
    virtual size_t getDirtyBlockSize() const = 0;

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
    virtual bool isDirty(GCAddress addr) const = 0;
    virtual bool isDirty(size_t index) const = 0;
    /*
     * ��ȡ�࿨
     */
    virtual size_t findDirty(GCAddress addr) const = 0;
    virtual size_t findDirty(size_t startOff) const = 0;
};
#endif