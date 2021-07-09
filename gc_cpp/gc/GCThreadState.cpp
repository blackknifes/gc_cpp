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
        {  //�������ٴμ�������ǣ�����Ѿ��˳�gc���̣���ֱ���˳�
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
        //��������������ͣ����
        m_locker.lock();
        // gc����ɣ����Ա���stw����
        if (!m_pauseRequested)
        {  //�������ٴμ�������ǣ�����Ѿ��˳�gc���̣���ֱ���˳�
            m_locker.unlock();
            return;
        }
        //�Ȼָ�������Ȼ�����ȴ�����֤�ȴ���ɺ󣬱�������ʼ��
        m_gcWaiter->wait(&m_locker);
        m_gcWaiter = nullptr;
        m_pauseRequested = false;

        m_safePoint = false;
        //�˳���ȫ��
        m_locker.unlock();
    }
    else
        m_safePoint = false;
}

void GCThreadState::waitEnterSafePoint()
{
    m_locker.lock();
    //���ð�ȫ��ȴ���
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
