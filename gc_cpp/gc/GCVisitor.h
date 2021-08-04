#ifndef __GCVISITOR_H__
#define __GCVISITOR_H__
#include <atomic>
#include <chrono>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "GCType.h"

template<typename _Ty>
class GCPtr;

class GarbageCollected;

class GCVisitor
{
public:
    virtual void visit(GarbageCollected* pGarbage) = 0;

    template<typename _Ty>
    void visit(const GCPtr<_Ty>& pGarbage)
    {
        visit(pGarbage.get());
    }

    template<typename _Itor>
    void visit(_Itor _begin, _Itor _end)
    {
        for (_Itor itor = _begin; itor != _end; ++itor) visit(*itor);
    }

    template<typename _Ty>
    void visit(const std::vector<GCPtr<_Ty>>& vec)
    {
        visit(vec.begin(), vec.end());
    }

    template<typename _Ty>
    void visit(const std::list<GCPtr<_Ty>>& vec)
    {
        visit(vec.begin(), vec.end());
    }

    template<typename _Ty>
    void visit(const std::deque<GCPtr<_Ty>>& vec)
    {
        visit(vec.begin(), vec.end());
    }

    template<typename _Ty>
    void visit(const std::set<GCPtr<_Ty>>& vec)
    {
        visit(vec.begin(), vec.end());
    }
    template<typename _kTy, typename _vTy>
    void visit(const std::map<_kTy, GCPtr<_vTy>>& _map)
    {
        for (auto itor = _map.begin(); itor != _map.end(); ++itor) visit(itor->second);
    }

    template<typename _kTy, typename _vTy>
    void visit(const std::map<GCPtr<_kTy>, _vTy>& _map)
    {
        for (auto itor = _map.begin(); itor != _map.end(); ++itor) visit(itor->first);
    }

    template<typename _kTy, typename _vTy>
    void visit(const std::map<GCPtr<_kTy>, GCPtr<_vTy>>& _map)
    {
        for (auto itor = _map.begin(); itor != _map.end(); ++itor)
        {
            visit(itor->first);
            visit(itor->second);
        }
    }

    template<typename _Ty>
    void visit(const std::unordered_set<GCPtr<_Ty>>& vec)
    {
        visit(vec.begin(), vec.end());
    }

    template<typename _kTy, typename _vTy>
    void visit(const std::unordered_map<_kTy, GCPtr<_vTy>>& _map)
    {
        for (auto itor = _map.begin(); itor != _map.end(); ++itor) visit(itor->second);
    }

    template<typename _kTy, typename _vTy>
    void visit(const std::unordered_map<GCPtr<_kTy>, _vTy>& _map)
    {
        for (auto itor = _map.begin(); itor != _map.end(); ++itor) visit(itor->first);
    }

    template<typename _kTy, typename _vTy>
    void visit(const std::unordered_map<GCPtr<_kTy>, GCPtr<_vTy>>& _map)
    {
        for (auto itor = _map.begin(); itor != _map.end(); ++itor)
        {
            visit(itor->first);
            visit(itor->second);
        }
    }
};

class GCVisitorFull : public GCVisitor
{
public:
    void visit(GarbageCollected* pGarbage) override;
};

class GCVisitorIncrement : public GCVisitor
{
public:
    static constexpr const size_t kCheckLoopCount = 100000;
    GCVisitorIncrement(uint64_t timeout);
    void visit(GarbageCollected* pGarbage) override;

    bool isFinish() const;

private:
    size_t m_couter;                 //标记计数
    uint64_t m_timeoutMicroSeconds;  //超时时间
    bool m_needReturn;
};

class GCVisitorCounted : public GCVisitor
{
public:
    GCVisitorCounted(GCVisitor* pVisitor);
    ~GCVisitorCounted();

    void visit(GarbageCollected* pGarbage) override;
    size_t getVisitCount() const;

private:
    GCVisitor* m_visitor;
    size_t m_counter;
};
#endif