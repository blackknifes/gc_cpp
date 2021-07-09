#ifndef __GCDELAYCONSTRUCT_H__
#define __GCDELAYCONSTRUCT_H__

template<typename _Ty>
class GCDelayConstruct
{
public:
    GCDelayConstruct(){};

    ~GCDelayConstruct(){};

    template<typename... _argsTy>
    void construct(_argsTy&&... args)
    {
        new (&m_value) _Ty(std::move(args)...);
    }

    void destroy()
    {
        m_value.~_Ty();
    }

    _Ty* operator->() const
    {
        return const_cast<_Ty*>(&m_value);
    }

    _Ty* operator&()
    {
        return &m_value;
    }
    const _Ty* operator&() const
    {
        return &m_value;
    }

private:
    union
    {
        _Ty m_value;
        unsigned char m_buffer[sizeof(_Ty)];
    };
};
#endif