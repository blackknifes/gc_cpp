#ifndef __GCMEMORYPOOL_H__
#define __GCMEMORYPOOL_H__
#include <malloc.h>

#include <vector>

#include "GCUtils.h"

template<typename _Ty>
class GCMemoryPool
{
public:
    GCMemoryPool() {}
    ~GCMemoryPool()
    {
        shrink();
    }

    template<typename... _argsTy>
    _Ty* pop(_argsTy&&... args)
    {
        _Ty* ptr;
        if (m_pool.empty())
            ptr = (_Ty*)malloc(sizeof(_Ty));
        else
        {
            ptr = m_pool.back();
            m_pool.pop_back();
        }
        GCUtils::Construct(ptr, std::move(args)...);
        return ptr;
    }

    void push(_Ty* ptr)
    {
        ptr->~_Ty();
        m_pool.push_back(ptr);
    }

    void shrink()
    {
        for (_Ty* ptr : m_pool) free(ptr);
        m_pool.clear();
        m_pool.shrink_to_fit();
    }

private:
    std::vector<_Ty*> m_pool;
};
#endif