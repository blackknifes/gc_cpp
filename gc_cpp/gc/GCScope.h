#ifndef __GCSCOPE_H__
#define __GCSCOPE_H__
#include <vector>

#include "GarbageCollected.h"

class GCScope
{
public:
    static GCScope* GetCurrent();

    GCScope();
    ~GCScope();

    GCScope* pre() const;
    GCScope* next() const;
    void addObject(GarbageCollected* pObject);

    void visit(GCVisitor* visitor);
private:
    GCScope* m_pre;
    GCScope* m_next;

    std::vector<GarbageCollected*> m_roots;

    friend class GCThreadState;
};
#endif