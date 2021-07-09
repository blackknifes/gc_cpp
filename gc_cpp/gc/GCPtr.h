#ifndef __GCPTR_H__
#define __GCPTR_H__
#include "GCThreadState.h"
#include "GCUnsafeScope.h"

template<typename _Ty>
class GCPtr
{
public:
    GCPtr() : m_pointer(nullptr){};

    GCPtr(_Ty* ptr) : m_pointer(nullptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr;
    }

    GCPtr(const GCPtr& ptr) : m_pointer(nullptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
    }

    template<typename _Ty2>
    GCPtr(const GCPtr<_Ty2>& ptr) : m_pointer(nullptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
    }

    const GCPtr& operator=(_Ty* ptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr;
        return *this;
    }

    const GCPtr& operator=(const GCPtr& ptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
        return *this;
    }

    template<typename _Ty2>
    const GCPtr& operator=(const GCPtr<_Ty2>& ptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
        return *this;
    }

    bool operator!() const
    {
        return !m_pointer;
    }

    operator bool() const
    {
        return m_pointer;
    }

    bool operator==(const _Ty* ptr) const
    {
        return m_pointer == ptr;
    }
    bool operator==(const GCPtr& ptr) const
    {
        return m_pointer == ptr.m_pointer;
    }
    template<typename _Ty2>
    bool operator==(const GCPtr<_Ty2>& ptr) const
    {
        return m_pointer == ptr.m_pointer;
    }

    bool operator!=(const _Ty* ptr) const
    {
        return m_pointer != ptr;
    }
    bool operator!=(const GCPtr& ptr) const
    {
        return m_pointer != ptr.m_pointer;
    }
    template<typename _Ty2>
    bool operator!=(const GCPtr<_Ty2>& ptr) const
    {
        return m_pointer != ptr.m_pointer;
    }

    _Ty* operator->() const
    {
        return m_pointer;
    }

    _Ty& operator*() const
    {
        return *m_pointer;
    }

    _Ty* get() const
    {
        return m_pointer;
    }

private:
    _Ty* m_pointer;
};
#endif