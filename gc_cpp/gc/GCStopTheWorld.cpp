#include "GCStopTheWorld.h"

GCStopTheWorld::GCStopTheWorld(): m_stopRequested(false) {}

void GCStopTheWorld::lock() const
{
    m_locker.lock();
}

void GCStopTheWorld::unlock() const
{
    m_locker.unlock();
}

void GCStopTheWorld::wait() const
{
    m_waiter.wait(&m_locker, INFINITE);
}

void GCStopTheWorld::notify() const
{
    m_waiter.notify();
}

void GCStopTheWorld::notifyAll() const
{
    m_waiter.notifyAll();
}

void GCStopTheWorld::pause() const
{
    m_stopRequested = true;
}

void GCStopTheWorld::resume() const
{
    m_stopRequested = false;
}

bool GCStopTheWorld::isStop() const
{
    return m_stopRequested;
}
