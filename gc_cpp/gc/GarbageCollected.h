#ifndef __GARBAGECOLLECTED_H__
#define __GARBAGECOLLECTED_H__
#include "GCVisitor.h"

class GarbageCollection
{
public:
    GarbageCollection();
    virtual ~GarbageCollection();
    virtual void gcTrace(GCVisitor* visitor);

    virtual size_t getObjectSize() const = 0;
    GarbageCollection* getObjectPointer() const;

    void gcPin() const;
    void gcUnpin() const;
    bool isPined() const;

private:
    friend class GCVisitor;
    friend class GCManager;

    void gcMark() const;
    void gcUnmark() const;
    bool isGcMarked() const;
    mutable bool m_pin;
    mutable bool m_mark;
};

template<typename _Class>
class GarbageCollected: public GarbageCollection
{
public:
    size_t getObjectSize() const override
    {
        return sizeof(_Class);
    }
};
#endif