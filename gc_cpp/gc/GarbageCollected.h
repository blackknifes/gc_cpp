#ifndef __GARBAGECOLLECTED_H__
#define __GARBAGECOLLECTED_H__
#include <atomic>

#include "GCType.h"
#include "GCVisitor.h"

class GarbageCollected;

class GarbageCollectedOperation
{
public:
    static void setColor(GarbageCollected* pObject, GCMarkColor color);
    static GCMarkColor getColor(GarbageCollected* pObject);
    static void setDirty(GarbageCollected* pObject, bool dirty);
    static bool isDirty(GarbageCollected* pObject);
    static void addAge(GarbageCollected* pObject);
    static size_t getAge(GarbageCollected* pObject);
};

class GarbageCollected
{
public:
    static constexpr const size_t kMarkColorMask =
        (size_t(1) << ((sizeof(size_t) * 8) - 1)) | (size_t(1) << ((sizeof(size_t) * 8) - 2));
    static constexpr const size_t kAgeMask = ~kMarkColorMask;

    GarbageCollected();
    virtual ~GarbageCollected();
    virtual void gcTrace(GCVisitor* visitor);

    GarbageCollected* getObjectPointer() const;

private:
    friend class GarbageCollectedOperation;
    mutable GCMarkFlag m_gcValue;
};
#endif