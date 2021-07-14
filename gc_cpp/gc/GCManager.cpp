#include "GCManager.h"

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
}

GCManager::~GCManager()
{
    s_manager = nullptr;
}

GCThreadState* GCManager::getThreadState(DWORD id) const
{
    m_threadsLocker.lock();
    auto itor = m_threads.find(id);
    if (itor == m_threads.end())
    {
        m_threadsLocker.unlock();
        return nullptr;
    }
    GCThreadState* pState = itor->second;
    m_threadsLocker.unlock();
    return pState;
}

void GCManager::addThreadState(GCThreadState* state)
{
    m_threadsLocker.lock();
    m_threads.insert({state->getThreadId(), state});
    m_threadsLocker.unlock();
}

void GCManager::removeThreadState(GCThreadState* state)
{
    m_threadsLocker.lock();
    m_garbages.insert(m_garbages.end(), state->m_garbages.begin(), state->m_garbages.end());
    m_threads.erase(state->getThreadId());
    m_threadsLocker.unlock();
}

void GCManager::stopWorld()
{
    m_threadsLocker.lock();
    m_stopFlag.pause();
    for (const auto& item : m_threads) item.second->waitEnterSafePoint();
    m_threadsLocker.unlock();
}

void GCManager::resumeWorld()
{
    m_stopFlag.lock();
    m_stopFlag.resume();
    m_stopFlag.notifyAll();
    m_stopFlag.unlock();

    for (GarbageCollected* pGarbage : m_delayCollected)
        delete pGarbage;
    m_delayCollected.clear();
}

void GCManager::printNumberOfSafePointThread() const
{
    m_threadsLocker.lock();
    int safeCount = 0;
    for (const auto& item : m_threads)
        if (item.second->isOnSafePoint()) ++safeCount;

    printf("number of thread on safe point: %d\n", safeCount);
    m_threadsLocker.unlock();
}

void GCManager::markSweep()
{
    GCVisitor visitor;
    m_threadsLocker.lock();
    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;
        for (const auto& item : pThreadState->m_roots)
        {
            GarbageCollected* pObject = item.second(*item.first);
            visitor.visit(pObject);
        }
    }
    for (const auto& item: m_roots)
    {
        GarbageCollected* pObject = item.second(*item.first);
        visitor.visit(pObject);
    }

    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;
        auto itor = pThreadState->m_garbages.begin();
        while (itor != pThreadState->m_garbages.end())
        {
            GarbageCollected* pGarbage = *itor;
            if (!pGarbage->isGcMarked())
            {
                itor = pThreadState->m_garbages.erase(itor);
                m_delayCollected.push_back(pGarbage);
            }
            else
            {
                pGarbage->gcUnmark();
                ++itor;
            }
        }
    }

    auto itor = m_garbages.begin();
    while (itor != m_garbages.end())
    {
        GarbageCollected* pGarbage = *itor;
        if (!pGarbage->isGcMarked())
        {
            itor = m_garbages.erase(itor);
            m_delayCollected.push_back(pGarbage);
        }
        else
        {
            pGarbage->gcUnmark();
            ++itor;
        }
    }
    m_threadsLocker.unlock();
}

const GCStopFlag* GCManager::getStopFlag() const
{
    return &m_stopFlag;
}

void GCManager::addRoot(void** ppAddress, PFN_Cast cast) 
{
    m_threadsLocker.lock();
    m_roots.insert({ppAddress, cast});
    m_threadsLocker.unlock();
}

void GCManager::removeRoot(void** ppAddress) 
{
    m_threadsLocker.lock();
    auto itor = m_roots.find(ppAddress);
    if (itor != m_roots.end()) m_roots.erase(itor);
    m_threadsLocker.unlock();
}
