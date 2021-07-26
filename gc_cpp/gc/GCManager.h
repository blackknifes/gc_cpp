#ifndef __GCMANAGER_H__
#define __GCMANAGER_H__
#include <unordered_map>
#include <vector>

#include "GCLocker.h"
#include "GCSettings.h"
#include "GCStopTheWorld.h"
#include "GCThreadState.h"
#include "GarbageCollected.h"

class GCMarkThread;
class GCSweepThread;

class GCManager
{
public:
    enum GCType
    {
        GC_MINOR,
        GC_FULL
    };

    static GCManager* GetGlobal();

    GCManager();
    ~GCManager();

    GCThreadState* getThreadState(DWORD id) const;

    void addThreadState(GCThreadState* state);
    void removeThreadState(GCThreadState* state);

    void gc(GCType gcType = GC_MINOR);

    //������ͣ
    void stopWorld();
    //����ָ�
    void resumeWorld();
    //���
    void mark(GCType gcType = GC_MINOR);
    //������
    void lazySweep();

    //�ȴ�����gc����
    void waitFinish();

    //��ӡ��ǰ���밲ȫ���̵߳�����
    void printNumberOfSafePointThread() const;

    const GCStopTheWorld* getStopFlag() const;

    void stopMarkThread();
    void stopSweepThread();

    GCSettings* getSettings() const;

    void addEden();
private:
    template<typename _Ty>
    friend class GCPersist;

    //������ͣ
    void stopWorldPrivate();
    //���
    void markPrivate(GCType markType);
    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void removeRoot(void** ppAddress);

    void checkSweepEden();
    void checkSweepSurvivor(size_t count);
    void checkSweepOld();

    GCLocker m_locker;              //��
    GCStopTheWorld m_stopTheWorld;  //ֹͣ��

    GCMarkThread* m_markThread;    // mark�߳�
    GCSweepThread* m_sweepThread;  // lazy sweep �߳�

    std::unordered_map<void**, PFN_Cast> m_roots;         //ȫ�ָ��ڵ�
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc�߳�

    std::vector<GarbageCollected*> m_willLazySweep;  //�ӳ������
    GCSettings* m_settings;                          // gc����

    std::list<GarbageCollected*> m_edenList;      //�����
    std::list<GarbageCollected*> m_survivorList;  //�����
    std::list<GarbageCollected*> m_oldList;       //�����
    std::atomic<size_t> m_edenCount;              //���������
};
#endif