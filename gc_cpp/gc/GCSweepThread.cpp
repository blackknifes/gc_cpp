#include "GCSweepThread.h"

GCSweepThread::GCSweepThread() : m_runFlag(true), m_needSweep(false) {}

void GCSweepThread::run()
{
    while (m_runFlag)
        doSweep();
    m_needSweep = true;
    doSweep();
}

void GCSweepThread::addGarbageList(std::vector<GarbageCollected*>& sweepList)
{
    m_locker.lock();
    m_willSweepList.insert(m_willSweepList.end(), sweepList.begin(), sweepList.end());
    m_waiter.notify();
    m_locker.unlock();
}

void GCSweepThread::setStop()
{
    m_locker.lock();
    m_runFlag = false;
    m_waiter.notify();
    m_locker.unlock();
}

void GCSweepThread::sweep()
{
    m_locker.lock();
    m_needSweep = true;
    m_waiter.notify();
    m_locker.unlock();
}

void GCSweepThread::doSweep()
{
    m_locker.lock();
    if (!m_needSweep) m_waiter.wait(&m_locker);
    m_currentSweepList.swap(m_willSweepList);
    m_needSweep = false;
    m_locker.unlock();
    for (GarbageCollected* pSweep : m_currentSweepList) delete pSweep;
    m_currentSweepList.clear();
}
