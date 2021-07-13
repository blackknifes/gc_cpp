#ifndef __GCMANAGER_H__
#define __GCMANAGER_H__
#include <unordered_map>
#include <vector>

#include "GCLocker.h"
#include "GCStopFlag.h"
#include "GCThreadState.h"
#include "GarbageCollected.h"

class GCManager
{
public:
    static GCManager* GetGlobal();

    GCManager();
    ~GCManager();

    GCThreadState* getThreadState(DWORD id) const;

    void addThreadState(GCThreadState* state);
    void removeThreadState(GCThreadState* state);

    //世界暂停
    void stopWorld();
    //世界恢复
    void resumeWorld();

    //打印当前进入安全点线程的数量
    void printNumberOfSafePointThread() const;

    void markSweep();

    const GCStopFlag* getStopFlag() const;

    //扫描根节点
    void scanRoots();

private:
    GCLocker m_threadsLocker;
    GCStopFlag m_stopFlag;
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc线程
    std::list<GarbageCollection*> m_garbages;
    std::vector<GarbageCollection*> m_delayCollected;
};
#endif