#include "GCThreadState.h"

#include <assert.h>

#include "GCManager.h"
#include "GarbageCollected.h"

namespace
{
    __declspec(thread) GCThreadState* s_threadState = nullptr;
}  // namespace

GCThreadState* GCThreadState::GetCurrent()
{
    return s_threadState;
}

GCThreadState::GCThreadState()
{
    assert(!s_threadState);
    m_scope = nullptr;
    s_threadState = this;
    m_safePoint = true;
    m_gcStopFlag = GCManager::GetGlobal()->getStopFlag();
    m_hThread = GetCurrentThread();

    GCManager::GetGlobal()->addThreadState(this);
}

GCThreadState::~GCThreadState()
{
    s_threadState = nullptr;
    GCManager::GetGlobal()->removeThreadState(this);
}

DWORD GCThreadState::getThreadId() const
{
    return GetThreadId(m_hThread);
}

void GCThreadState::enterSafePoint()
{
    m_safePoint = true;
}

void GCThreadState::leaveSafePoint()
{
    m_safePoint = false;
    if (m_gcStopFlag->isStop())
    {
        //加锁，并进入暂停流程
        m_gcStopFlag->lock();
        // gc已完成，忽略本次stw请求
        if (!m_gcStopFlag->isStop())
        {  //加锁后再次检查请求标记，如果已经退出gc流程，则直接退出
            m_gcStopFlag->unlock();
            return;
        }
        m_safePoint = true;
        //先恢复变量，然后进入等待，保证等待完成后，变量被初始化
        m_gcStopFlag->wait();
        m_safePoint = false;
        //退出安全点
        m_gcStopFlag->unlock();
        destroyGarbage();
    }
}

void GCThreadState::waitEnterSafePoint()
{
    bool result;
    //设置安全点等待器
    do
    {
        m_gcStopFlag->lock();
        result = m_safePoint;
        m_gcStopFlag->unlock();
        if (!result) SwitchToThread();
    }
    while (!result);
}

void GCThreadState::addGarbage(GarbageCollected* pGarbage)
{
    m_garbages.push_back(pGarbage);
}

bool GCThreadState::isOnSafePoint() const
{
    return m_safePoint;
}

void GCThreadState::destroyGarbage()
{
    auto itor = m_garbages.begin();
    while (itor != m_garbages.end())
    {
        GarbageCollected* pGarbage = *itor;
        if (!pGarbage->isGcMarked())
        {
            m_delayDestroy.push_back(*itor);
            itor = m_garbages.erase(itor);
        }
        else
        {
            pGarbage->gcUnmark();
            ++itor;
        }
    }
    for (GarbageCollected* pObject : m_delayDestroy) delete pObject;
    m_delayDestroy.clear();
}

void GCThreadState::enterScope(GCScope* pScope)
{
    pScope->m_pre = m_scope;
    if (m_scope) m_scope->m_next = pScope;
    m_scope = pScope;
}

void GCThreadState::leaveScope()
{
    m_scope = m_scope->pre();
    if (m_scope) m_scope->m_next = nullptr;
}

GCScope* GCThreadState::getScope() const
{
    return m_scope;
}

void GCThreadState::addRoot(void** ppAddress, PFN_Cast cast)
{
    m_roots.push_back({ppAddress, cast});
}

void GCThreadState::popRoot()
{
    m_roots.pop_back();
}
