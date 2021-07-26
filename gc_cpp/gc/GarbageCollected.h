#ifndef __GARBAGECOLLECTED_H__
#define __GARBAGECOLLECTED_H__
#include <atomic>

#include "GCVisitor.h"

class GarbageCollected
{
public:
    static constexpr const size_t kMarkMask = size_t(1) << ((sizeof(size_t) * 8) - 1);
    static constexpr const size_t kAgeMask = ~kMarkMask;

    GarbageCollected();
    virtual ~GarbageCollected();
    virtual void gcTrace(GCVisitor* visitor);

    GarbageCollected* getObjectPointer() const;

private:
    friend class GCVisitor;
    friend class GCManager;
    friend class GCThreadState;
    template<typename _Ty>
    friend class GCPersist;

    void gcMark() const;
    void gcUnmark() const;
    bool isGcMarked() const;

    void addGCAge() const;
    size_t getGCAge() const;

    mutable size_t m_gcValue;
};
#endif