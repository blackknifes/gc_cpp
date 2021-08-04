#ifndef __GCLIST_H__
#define __GCLIST_H__
#include "GCMemoryPool.h"
#include <assert.h>

template<typename _Ty>
class GCList;

template<typename _NodeTy>
class GCListIterator
{
private:
    _NodeTy* m_pre;
    _NodeTy* m_node;

public:
    GCListIterator(_NodeTy* pre, _NodeTy* node) : m_pre(pre), m_node(node) {}

    GCListIterator& operator++()
    {
        m_pre = m_node;
        m_node = m_node->next();
        return *this;
    }

    GCListIterator operator++(int)
    {
        return operator++();
    }

    GCListIterator& operator--()
    {
        m_node = m_pre;
        m_pre = m_pre->pre();
        return *this;
    }

    GCListIterator operator--(int)
    {
        return operator--();
    }

    auto value() const -> decltype(m_node->value())
    {
        return m_node->value();
    }

    auto operator*() const -> decltype(this->value())
    {
        return this->value();
    }

    bool operator==(const GCListIterator& itor) const
    {
        return m_node == itor.m_node;
    }
    bool operator!=(const GCListIterator& itor) const
    {
        return !operator==(itor);
    }
};

template<typename _Ty>
class GCList
{
private:
    class Node
    {
    public:
        Node() : m_next(nullptr), m_pre(nullptr) {}
        Node(const _Ty& val) : m_value(val), m_next(nullptr), m_pre(nullptr) {}
        Node(_Ty&& val) : m_value(std::move(val)), m_next(nullptr), m_pre(nullptr) {}

        void linkAfter(Node* pNode)
        {
            m_next = pNode;
            pNode->m_pre = this;
        }
        void linkBefore(Node* pNode)
        {
            pNode->m_next = this;
            m_pre = pNode;
        }

        Node* next() const
        {
            return m_next;
        }
        Node* pre() const
        {
            return m_pre;
        }
        _Ty& value()
        {
            return m_value;
        }
        const _Ty& value() const
        {
            return m_value;
        }

    private:
        _Ty m_value;
        Node* m_next;
        Node* m_pre;
    };

public:
    typedef GCListIterator<Node> iterator;
    typedef GCListIterator<const Node> const_iterator;

    GCList() : m_front(nullptr), m_back(nullptr), m_size(0) {}

    ~GCList()
    {
        clear();
    }

    GCList(const GCList& _list)
    {
        for (const _Ty& val : _list) push_back(val);
    }
    GCList(GCList&& _list) : m_front(_list.m_front), m_back(_list.m_back), m_size(_list.m_size)
    {
        _list.m_front = _list.m_back = nullptr;
        _list.m_size = 0;
    }

    const GCList& operator=(const GCList& _list)
    {
        clear();
        for (const _Ty& val : _list) push_back(val);
        return *this;
    }
    const GCList& operator=(GCList&& _list)
    {
        clear();
        m_pool.shrink();
        m_front = _list.m_front;
        m_back = _list.m_back;
        m_size = _list.m_size;
        _list.m_front = _list.m_back = nullptr;
        _list.m_size = 0;
        return *this;
    }

    void swap(GCList& _list)
    {
        std::swap(m_front, _list.m_front);
        std::swap(m_back, _list.m_back);
        std::swap(m_size, _list.m_size);
    }

    void push_back(const _Ty& val)
    {
        Node* pNode = m_pool.pop(val);
        if (!m_back)
            m_front = m_back = pNode;
        else
        {
            m_back->linkAfter(pNode);
            m_back = pNode;
        }
        ++m_size;
    }
    void push_back(_Ty&& val)
    {
        Node* pNode = m_pool.pop(std::move(val));
        if (!m_back)
            m_front = m_back = pNode;
        else
        {
            m_back->linkAfter(pNode);
            m_back = pNode;
        }
        ++m_size;
    }
    void push_front(const _Ty& val)
    {
        Node* pNode = m_pool.pop(val);
        if (!m_front)
            m_front = m_back = pNode;
        else
        {
            m_front->linkBefore(pNode);
            m_front = pNode;
        }
        ++m_size;
    }
    void push_front(_Ty&& val)
    {
        Node* pNode = m_pool.pop(std::move(val));
        if (!m_front)
            m_front = m_back = pNode;
        else
        {
            m_front->linkBefore(pNode);
            m_front = pNode;
        }

        ++m_size;
    }

    _Ty& front()
    {
        return m_front->value();
    }
    const _Ty& front() const
    {
        return m_front->value();
    }
    _Ty& back()
    {
        return m_back->value();
    }
    const _Ty& back() const
    {
        return m_back->value();
    }

    void pop_back()
    {
        Node* pNode = m_back;
        m_back = m_back->pre();
        if (!m_back) m_front = nullptr;
        m_pool.push(pNode);
        --m_size;
    }
    void pop_front()
    {
        Node* pNode = m_front;
        m_front = m_front->next();
        if (!m_front) m_back = nullptr;
        m_pool.push(pNode);
        --m_size;
    }

    void clear()
    {
        Node* pNode = m_front;
        while (pNode)
        {
            Node* pNext = pNode->next();
            m_pool.push(pNode);
            pNode = pNext;
        }
        m_size = 0;
        m_front = m_back = nullptr;
    }

    bool empty() const
    {
        return m_size == 0;
    }

    iterator begin()
    {
        return iterator(nullptr, m_front);
    }
    iterator end()
    {
        return iterator(m_back, nullptr);
    }
    const_iterator begin() const
    {
        return cbegin();
    }
    const_iterator end() const
    {
        return cend();
    }
    const_iterator cbegin() const
    {
        return const_iterator(nullptr, m_front);
    }
    const_iterator cend() const
    {
        return const_iterator(m_back, nullptr);
    }

private:
    friend class GCListIterator<Node>;
    friend class GCListIterator<const Node>;

    GCMemoryPool<Node> m_pool;
    Node* m_front;
    Node* m_back;
    size_t m_size;
};
#endif