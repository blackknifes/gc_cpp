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
    //懒清理
    void lazySweep();

    //打印当前进入安全点线程的数量
    void printNumberOfSafePointThread() const;

    void markSweep();

    const GCStopFlag* getStopFlag() const;

private:
    template<typename _Ty>
    friend class GCPersist;

    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void removeRoot(void** ppAddress);

    GCLocker m_threadsLocker;
    GCStopFlag m_stopFlag;
    std::unordered_map<void**, PFN_Cast> m_roots;         //全局根节点
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc线程
    std::list<GarbageCollected*> m_garbages;
    std::vector<GarbageCollected*> m_delayCollected;
};
#endif