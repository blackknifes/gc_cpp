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
    m_threads.erase(state->getThreadId());
    m_threadsLocker.unlock();
}

void GCManager::stopWorld()
{
    m_threadsLocker.lock();
    for (const auto& item : m_threads) item.second->pause(&m_gcWaiter);
    for (const auto& item : m_threads) item.second->waitEnterSafePoint();
    m_threadsLocker.unlock();
}

void GCManager::resumeWorld()
{
    m_threadsLocker.lock();
    for (const auto& item : m_threads) item.second->resume();
    m_gcWaiter.notifyAll();
    m_threadsLocker.unlock();
    for (GarbageCollection* pGarbage : m_delayCollected)
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
        GCScope* pScope = pThreadState->getScope();
        while (pScope)
        {
            for (GarbageCollection* pGarbage : pScope->m_garbageList) visitor.visit(pGarbage);
            pScope = pScope->next();
        }
    }

    for (const auto& item : m_threads)
    {
        GCThreadState* pThreadState = item.second;
        auto itor = pThreadState->m_garbages.begin();
        while (itor != pThreadState->m_garbages.end())
        {
            GarbageCollection* pGarbage = *itor;
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
    m_threadsLocker.unlock();
}
