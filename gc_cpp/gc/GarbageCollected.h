#ifndef __GARBAGECOLLECTED_H__
#define __GARBAGECOLLECTED_H__
#include <atomic>

#include "GCVisitor.h"

enum GCMarkColor : uint8_t
{
    GC_MARK_WHITE = 0,
    GC_MARK_BLACK,
    GC_MARK_GRAY
};

struct GCMarkFlag
{
    GCMarkColor color : 2;
    size_t age : sizeof(size_t) - 2;
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
    friend class GCVisitor;
    friend class GCManager;
    friend class GCThreadState;
    template<typename _Ty>
    friend class GCPersist;

    void gcSetColor(GCMarkColor color = GC_MARK_BLACK) const;
    GCMarkColor getGcMarkColor() const;

    void addGCAge() const;
    size_t getGCAge() const;

    mutable GCMarkFlag m_gcValue;
};
#endif