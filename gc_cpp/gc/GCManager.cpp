#include "GCManager.h"

#include "GCMarkThread.h"
#include "GCSweepThread.h"
#include "GCTimer.h"
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
    m_settings = new GCSettings();
    m_markThread = new GCMarkThread(this);
    m_sweepThread = new GCSweepThread();
    m_markThread->start(0x10000);
    m_sweepThread->start(0x10000);
}

GCManager::~GCManager()
{
    waitFinish();
    delete m_settings;
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
    m_edenList.insert(m_edenList.end(), state->m_garbages.begin(), state->m_garbages.end());
    m_threads.erase(state->getThreadId());
    m_locker.unlock();
}

void GCManager::gc(GCType gcType /*= GC_MINOR*/)
{
    m_markThread->gc(gcType);
}

void GCManager::stopWorld()
{
    if (m_settings->isVerbose())
    {
        GCTimer timer;
        stopWorldPrivate();
        printf("%s: %lld ms\n", __FUNCTION__, timer.getMilliSeconds());
    }
    else
        stopWorldPrivate();
}

void GCManager::stopWorldPrivate()
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

void GCManager::mark(GCType gcType /*= GC_MINOR*/)
{
    if (m_settings->isVerbose())
    {
        GCTimer timer;
        markPrivate(gcType);
        printf("%s: %lld ms\n", __FUNCTION__, timer.getMilliSeconds());
    }
    else
        markPrivate(gcType);
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
    gc(GC_FULL);
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

void GCManager::markPrivate(GCType markType)
{
    GCVisitor visitor;
    GCVisitorCounted visitorCounted;
    GCVisitor* pVisitor = m_settings->isVerbose() ? &visitorCounted : &visitor;

    m_locker.lock();
    //扫描所有线程所持根节点以及persist节点
    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;
        for (const auto& item : pThreadState->m_roots)
        {
            GarbageCollected* pObject = item.second(*item.first);
            pVisitor->visit(pObject);
        }

        GCScope* pScope = pThreadState->getScope();
        while (pScope)
        {
            pScope->visit(pVisitor);
            pScope = pScope->pre();
        }
    }

    //扫描全局persist节点
    for (const auto& item : m_roots)
    {
        GarbageCollected* pObject = item.second(*item.first);
        pVisitor->visit(pObject);
    }

    //处理所有线程所持垃圾堆
    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;

        m_edenList.insert(m_edenList.end(),
                          pThreadState->m_garbages.begin(),
                          pThreadState->m_garbages.end());
        pThreadState->m_garbages.clear();
    }

    if (markType == GC_FULL)
    {
        checkSweepOld();
        checkSweepSurvivor(SIZE_MAX);
        checkSweepEden();
    }
    else
    {
        size_t count = m_survivorList.size();
        checkSweepEden();
        if (m_survivorList.size() > m_settings->getSurvisorThreshold())
        {
            checkSweepOld();
            checkSweepSurvivor(count);
        }
    }
    m_edenCount = m_edenList.size();
    m_locker.unlock();

    if (m_settings->isVerbose()) printf("mark count: %zu\n", visitorCounted.getVisitCount());
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

GCSettings* GCManager::getSettings() const
{
    return m_settings;
}

void GCManager::addEden()
{
    if (++m_edenCount >= m_settings->getEdenThreshold()) gc();
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

void GCManager::checkSweepEden()
{  //年轻代标记
    auto itor = m_edenList.begin();
    while (itor != m_edenList.end())
    {
        GarbageCollected* pGarbage = *itor;
        if (pGarbage->getGcMarkColor() == GC_MARK_WHITE)
        {  //未被标记，加入延迟清理列表
            itor = m_edenList.erase(itor);
            m_willLazySweep.push_back(pGarbage);
        }
        else
        {
            //已被标记，清理标记
            pGarbage->gcSetColor(GC_MARK_WHITE);
            pGarbage->addGCAge();
            ++itor;
            m_survivorList.push_back(pGarbage);
        }
    }
    m_edenList.clear();
}

void GCManager::checkSweepSurvivor(size_t count)
{
    //中年代标记
    size_t i = 0;
    auto itor = m_survivorList.begin();
    while (itor != m_survivorList.end() && i < count)
    {
        ++i;
        GarbageCollected* pGarbage = *itor;
        if (pGarbage->getGcMarkColor() == GC_MARK_WHITE)
        {  //未被标记，加入延迟清理列表
            itor = m_survivorList.erase(itor);
            m_willLazySweep.push_back(pGarbage);
        }
        else
        {
            //已被标记，清理标记
            pGarbage->gcSetColor(GC_MARK_WHITE);
            pGarbage->addGCAge();
            if (pGarbage->getGCAge() >= 15)
            {
                itor = m_survivorList.erase(itor);
                m_oldList.push_back(pGarbage);
            }
            else
                ++itor;
        }
    }
}

void GCManager::checkSweepOld()
{
    //年轻代标记
    auto itor = m_oldList.begin();
    while (itor != m_oldList.end())
    {
        GarbageCollected* pGarbage = *itor;
        if (pGarbage->getGcMarkColor() == GC_MARK_WHITE)
        {  //未被标记，加入延迟清理列表
            itor = m_oldList.erase(itor);
            m_willLazySweep.push_back(pGarbage);
        }
        else
        {
            //已被标记，清理标记
            pGarbage->gcSetColor(GC_MARK_WHITE);
            ++itor;
        }
    }
}
