#ifndef __GCEXECUTORFULL_H__
#define __GCEXECUTORFULL_H__
#include <vector>

#include "GCExecutor.h"
#include "GCVisitor.h"

/**
 * ÍêÕûgcÖ´ÐÐÆ÷
 */
class GCExecutorFull : public GCExecutor, public GCVisitor
{
public:
    void scanRoot() override;

    void mark() override;

    void sweep() override;

    void visit(GarbageCollected* pGarbage) override;

private:
    std::vector<GarbageCollected*> m_roots;
};
#endif