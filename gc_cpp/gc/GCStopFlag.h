#ifndef __GCSTOPFLAG_H__
#define __GCSTOPFLAG_H__
#include "GCLocker.h"
#include "GCWaiter.h"

class GCStopFlag
{
public:
    GCStopFlag();
	
    void lock() const;
    void unlock() const;

    void wait() const;
    void notify() const;
    void notifyAll() const;

    void pause() const;
    void resume() const;

    bool isStop() const;
private:
    mutable bool m_stopRequested;
    GCLocker m_locker;
    GCWaiter m_waiter;
};
#endif