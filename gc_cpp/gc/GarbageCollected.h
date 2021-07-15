#ifndef __GARBAGECOLLECTED_H__
#define __GARBAGECOLLECTED_H__
#include <atomic>

#include "GCVisitor.h"

class GarbageCollected
{
public:
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

    mutable std::atomic<bool> m_mark;
};
#endif