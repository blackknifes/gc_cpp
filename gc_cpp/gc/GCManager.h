#ifndef __GCMANAGER_H__
#define __GCMANAGER_H__
#include <unordered_map>
#include <vector>

#include "GCLocker.h"
#include "GCStopTheWorld.h"
#include "GCThreadState.h"
#include "GarbageCollected.h"

class GCMarkThread;
class GCSweepThread;

class GCManager
{
public:
    static GCManager* GetGlobal();

    GCManager();
    ~GCManager();

    GCThreadState* getThreadState(DWORD id) const;

    void addThreadState(GCThreadState* state);
    void removeThreadState(GCThreadState* state);

    void gc();

    //世界暂停
    void stopWorld();
    //世界恢复
    void resumeWorld();
    //标记
    void mark();
    //懒清理
    void lazySweep();

    //等待最后的gc结束
    void waitFinish();

    //打印当前进入安全点线程的数量
    void printNumberOfSafePointThread() const;

    const GCStopTheWorld* getStopFlag() const;


    void stopMarkThread();
    void stopSweepThread();

private:
    template<typename _Ty>
    friend class GCPersist;

    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void removeRoot(void** ppAddress);

    GCLocker m_locker;          //锁
    GCStopTheWorld m_stopTheWorld;  //停止器

    GCMarkThread* m_markThread;    // mark线程
    GCSweepThread* m_sweepThread;  // lazy sweep 线程

    std::unordered_map<void**, PFN_Cast> m_roots;         //全局根节点
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc线程
    std::list<GarbageCollected*> m_garbages;              //全局垃圾堆
    std::vector<GarbageCollected*> m_willLazySweep;       //延迟清理表
};
#endif