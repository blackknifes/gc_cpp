#include "GCThreadState.h"

#include <assert.h>

#include "GCManager.h"
#include "GarbageCollected.h"

namespace
{
    __declspec(thread) GCThreadState* s_threadState = nullptr;
}

GCThreadState* GCThreadState::GetCurrent()
{
    return s_threadState;
}

GCThreadState::GCThreadState()
{
    assert(!s_threadState);
    s_threadState = this;
    m_pauseRequested = false;
    m_safePoint = true;
    GCManager::GetGlobal();
    m_hThread = GetCurrentThread();
    m_firstScope = m_lastScope = nullptr;
    m_rootScope.construct();
    m_firstScope = m_lastScope = &m_rootScope;

    GCManager::GetGlobal()->addThreadState(this);
}

GCThreadState::~GCThreadState()
{
    m_rootScope.destroy();
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
    if (m_pauseRequested)
    {
        m_locker.lock();
        if (!m_pauseRequested)
        {  //加锁后再次检查请求标记，如果已经退出gc流程，则直接退出
            m_locker.unlock();
            return;
        }
        m_safePointWaiter.notify();
        m_locker.unlock();
    }
}

void GCThreadState::leaveSafePoint()
{
    if (m_pauseRequested)
    {
        //加锁，并进入暂停流程
        m_locker.lock();
        // gc已完成，忽略本次stw请求
        if (!m_pauseRequested)
        {  //加锁后再次检查请求标记，如果已经退出gc流程，则直接退出
            m_locker.unlock();
            return;
        }
        //先恢复变量，然后进入等待，保证等待完成后，变量被初始化
        m_gcWaiter->wait(&m_locker);
        m_gcWaiter = nullptr;
        m_pauseRequested = false;

        m_safePoint = false;
        //退出安全点
        m_locker.unlock();
    }
    else
        m_safePoint = false;
}

void GCThreadState::waitEnterSafePoint()
{
    m_locker.lock();
    //设置安全点等待器
    if (!m_safePoint) m_safePointWaiter.wait(&m_locker);
    m_locker.unlock();
}

void GCThreadState::pause(GCWaiter* pGcWaiter)
{
    m_locker.lock();
    m_gcWaiter = pGcWaiter;
    m_pauseRequested = true;
    m_locker.unlock();
}

void GCThreadState::resume()
{
    m_locker.lock();
    m_pauseRequested = false;
    m_locker.unlock();
}

void GCThreadState::addGarbage(GarbageCollection* pGarbage)
{
    m_garbages.push_back(pGarbage);
}

GCScope* GCThreadState::getScope() const
{
    return m_lastScope;
}

bool GCThreadState::isOnSafePoint() const
{
    return m_safePoint;
}
