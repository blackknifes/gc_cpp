#include "GCMarkThread.h"

GCMarkThread::GCMarkThread(GCManager* pManager)
    : m_manager(pManager),
      m_runFlag(true),
      m_needGC(false)
{
}

void GCMarkThread::run()
{
    while (m_runFlag) doMark();
    m_needGC = true;
    doMark();
    m_manager->stopSweepThread();
}

void GCMarkThread::setStop()
{
    m_locker.lock();
    m_runFlag = false;
    m_waiter.notify();
    m_locker.unlock();
}

void GCMarkThread::gc(GCManager::GCType gcType /*= GCManager::GC_MINOR*/)
{
    m_locker.lock();
    m_needGC = true;
    m_gcType = gcType;
    m_waiter.notify();
    m_locker.unlock();
}

void GCMarkThread::doMark()
{
    m_locker.lock();

    if (!m_needGC) m_waiter.wait(&m_locker, (DWORD)m_manager->getSettings()->getGCPeriod());
    m_manager->stopWorld();
    m_manager->mark(m_gcType);
    m_manager->resumeWorld();
    m_needGC = false;
    m_locker.unlock();
    m_manager->lazySweep();
}
