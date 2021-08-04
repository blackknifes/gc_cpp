#include "GCExecutorFull.h"

#include "GCManager.h"
#include "GCThreadState.h"

void GCExecutorFull::scanRoot()
{
    //没有扫描截断，直接进入mark阶段
    setPhase(GC_PHASE_MARK);
}

void GCExecutorFull::mark() 
{
    GCManager* pManager = GCManager::GetGlobal();
    pManager->stopMarkThread();

    auto cb = [this](GarbageCollected* pObject)
    {
        visit(pObject);
    };
    pManager->enumRoots(cb);
    pManager->enumThreadState(
        [&cb](GCThreadState* pState)
        {
            pState->enumRoots(cb);
            pState->enumScopePersist(cb);
        });

    std::list<GarbageCollected*> objs;
    objs.swap(pManager->getGarbages());
    pManager->resumeWorld();
}

void GCExecutorFull::sweep() 
{

}

void GCExecutorFull::visit(GarbageCollected* pGarbage)
{
    //已经标记，跳出
    if (GarbageCollectedOperation::getColor(pGarbage) != GC_MARK_WHITE) return;
    //标记对象
    GarbageCollectedOperation::setColor(pGarbage, GC_MARK_BLACK);
    //继续追踪
    pGarbage->gcTrace(this);
}
