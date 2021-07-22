#ifndef __GCTHREADSTATE_H__
#define __GCTHREADSTATE_H__
#include <Windows.h>

#include <atomic>
#include <list>
#include <vector>

#include "GCDelayConstruct.h"
#include "GCLocker.h"
#include "GCScope.h"
#include "GCStopFlag.h"
#include "GCWaiter.h"

class GarbageCollected;

class GCThreadState
{
public:
    static GCThreadState* GetCurrent();

    GCThreadState();
    ~GCThreadState();

    DWORD getThreadId() const;

    void enterSafePoint();
    void leaveSafePoint();
    void waitEnterSafePoint();

    void addGarbage(GarbageCollected* pGarbage);
    bool isOnSafePoint() const;

    void destroyGarbage();

    void enterScope(GCScope* pScope);
    void leaveScope();

    GCScope* getScope() const;
private:
    friend class GCManager;
    template<typename _Ty>
    friend class GCPersist;

    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void popRoot();

    HANDLE m_hThread;                //�߳̾��
    const GCStopFlag* m_gcStopFlag;  // gc�ȴ���
    std::vector<std::pair<void**, PFN_Cast>> m_roots;

    GCScope* m_scope;

    std::list<GarbageCollected*> m_garbages;        //�ȴ��ռ�������
    std::vector<GarbageCollected*> m_delayDestroy;  //�ӳ�����
    std::atomic<bool> m_safePoint;
};
#endif