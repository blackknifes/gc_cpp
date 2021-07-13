#include "GCStopFlag.h"

GCStopFlag::GCStopFlag(): m_stopRequested(false) {}

void GCStopFlag::lock() const
{
    m_locker.lock();
}

void GCStopFlag::unlock() const
{
    m_locker.unlock();
}

void GCStopFlag::wait() const
{
    m_waiter.wait(&m_locker, INFINITE);
}

void GCStopFlag::notify() const
{
    m_waiter.notify();
}

void GCStopFlag::notifyAll() const
{
    m_waiter.notifyAll();
}

void GCStopFlag::pause() const
{
    m_stopRequested = true;
}

void GCStopFlag::resume() const
{
    m_stopRequested = false;
}

bool GCStopFlag::isStop() const
{
    return m_stopRequested;
}
