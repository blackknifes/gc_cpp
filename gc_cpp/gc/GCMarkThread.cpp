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

void GCMarkThread::gc()
{
    m_locker.lock();
    m_needGC = true;
    m_waiter.notify();
    m_locker.unlock();
}

void GCMarkThread::doMark()
{
    m_locker.lock();

    if (!m_needGC) m_waiter.wait(&m_locker);
    m_manager->stopWorld();
    m_manager->mark();
    m_manager->resumeWorld();
    m_needGC = false;
    m_locker.unlock();
    m_manager->lazySweep();
}
