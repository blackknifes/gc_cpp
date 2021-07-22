#include "GCManager.h"

#include "GCMarkThread.h"
#include "GCSweepThread.h"
#include "GCVisitor.h"

namespace
{
    GCManager* s_manager = nullptr;
}

GCManager* GCManager::GetGlobal()
{
    return s_manager;
}

GCManager::GCManager()
{
    s_manager = this;
    m_markThread = new GCMarkThread(this);
    m_sweepThread = new GCSweepThread();
    m_markThread->start(0x10000);
    m_sweepThread->start(0x10000);
}

GCManager::~GCManager()
{
    waitFinish();
    delete m_markThread;
    delete m_sweepThread;
    s_manager = nullptr;
}

GCThreadState* GCManager::getThreadState(DWORD id) const
{
    m_locker.lock();
    auto itor = m_threads.find(id);
    if (itor == m_threads.end())
    {
        m_locker.unlock();
        return nullptr;
    }
    GCThreadState* pState = itor->second;
    m_locker.unlock();
    return pState;
}

void GCManager::addThreadState(GCThreadState* state)
{
    m_locker.lock();
    m_threads.insert({state->getThreadId(), state});
    m_locker.unlock();
}

void GCManager::removeThreadState(GCThreadState* state)
{
    m_locker.lock();
    m_garbages.insert(m_garbages.end(), state->m_garbages.begin(), state->m_garbages.end());
    m_threads.erase(state->getThreadId());
    m_locker.unlock();
}

void GCManager::gc()
{
    m_markThread->gc();
}

void GCManager::stopWorld()
{
    m_locker.lock();
    m_stopTheWorld.pause();
    for (const auto& item : m_threads) item.second->waitEnterSafePoint();
    m_locker.unlock();
}

void GCManager::resumeWorld()
{
    m_stopTheWorld.lock();
    m_stopTheWorld.resume();
    m_stopTheWorld.notifyAll();
    m_stopTheWorld.unlock();
}

void GCManager::lazySweep()
{
    m_locker.lock();
    //加入延迟加载任务，并启动延迟清理
    m_sweepThread->addGarbageList(m_willLazySweep);
    m_willLazySweep.clear();
    m_sweepThread->sweep();
    m_locker.unlock();
}

void GCManager::waitFinish()
{
    gc();
    stopMarkThread();
    GCThread* threads[] = {m_markThread, m_sweepThread};
    GCThread::JoinAll(threads, sizeof(threads) / sizeof(threads[0]));
}

void GCManager::printNumberOfSafePointThread() const
{
    m_locker.lock();
    int safeCount = 0;
    for (const auto& item : m_threads)
        if (item.second->isOnSafePoint()) ++safeCount;

    printf("number of thread on safe point: %d\n", safeCount);
    m_locker.unlock();
}

void GCManager::mark()
{
    GCVisitor visitor;
    m_locker.lock();
    //扫描所有线程所持根节点以及persist节点
    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;
        for (const auto& item : pThreadState->m_roots)
        {
            GarbageCollected* pObject = item.second(*item.first);
            visitor.visit(pObject);
        }

        GCScope* pScope = pThreadState->getScope();
        while (pScope)
        {
            pScope->visit(&visitor);
            pScope = pScope->pre();
        }
    }

    //扫描全局persist节点
    for (const auto& item : m_roots)
    {
        GarbageCollected* pObject = item.second(*item.first);
        visitor.visit(pObject);
    }

    //处理所有线程所持垃圾堆
    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;

        auto itor = pThreadState->m_garbages.begin();
        while (itor != pThreadState->m_garbages.end())
        {
            GarbageCollected* pGarbage = *itor;
            if (!pGarbage->isGcMarked())
            {  //未被标记，加入延迟清理列表
                itor = pThreadState->m_garbages.erase(itor);
                m_willLazySweep.push_back(pGarbage);
            }
            else
            {
                //已被标记，清理标记
                pGarbage->gcUnmark();
                ++itor;
            }
        }
    }

    //处理全局垃圾堆
    auto itor = m_garbages.begin();
    while (itor != m_garbages.end())
    {
        GarbageCollected* pGarbage = *itor;
        if (!pGarbage->isGcMarked())
        {
            itor = m_garbages.erase(itor);
            m_willLazySweep.push_back(pGarbage);
        }
        else
        {
            pGarbage->gcUnmark();
            ++itor;
        }
    }
    m_locker.unlock();
}

const GCStopTheWorld* GCManager::getStopFlag() const
{
    return &m_stopTheWorld;
}

void GCManager::stopMarkThread()
{
    m_markThread->setStop();
}

void GCManager::stopSweepThread()
{
    m_sweepThread->setStop();
}

void GCManager::addRoot(void** ppAddress, PFN_Cast cast)
{
    m_locker.lock();
    m_roots.insert({ppAddress, cast});
    m_locker.unlock();
}

void GCManager::removeRoot(void** ppAddress)
{
    m_locker.lock();
    auto itor = m_roots.find(ppAddress);
    if (itor != m_roots.end()) m_roots.erase(itor);
    m_locker.unlock();
}
