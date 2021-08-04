#include "GCExecutorFull.h"

#include "GCManager.h"
#include "GCThreadState.h"

void GCExecutorFull::scanRoot()
{
    //û��ɨ��ضϣ�ֱ�ӽ���mark�׶�
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
    //�Ѿ���ǣ�����
    if (GarbageCollectedOperation::getColor(pGarbage) != GC_MARK_WHITE) return;
    //��Ƕ���
    GarbageCollectedOperation::setColor(pGarbage, GC_MARK_BLACK);
    //����׷��
    pGarbage->gcTrace(this);
}
