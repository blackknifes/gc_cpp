#ifndef __GCVISITOR_H__
#define __GCVISITOR_H__
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

template<typename _Ty>
class GCPtr;

class GarbageCollection;

class GCVisitor
{
public:
    virtual void visit(GarbageCollection* pGarbage);

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

    template<typename _Ty>
    void visit(const std::unordered_set<GCPtr<_Ty>>& vec)
    {
        visit(vec.begin(), vec.end());
    }
};
#endif