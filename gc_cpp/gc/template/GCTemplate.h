#ifndef __GCTEMPLATE_H__
#define __GCTEMPLATE_H__
#include <xutility>

class GCUtil
{
public:
    template<typename _Ty, typename... _argsTy>
    static void Construct(_Ty* ptr, _argsTy&&... args)
    {
        new (ptr) _Ty(std::move(args)...);
    }
    template<typename _Ty>
    static void Destroy(_Ty* ptr)
    {
        ptr->~_Ty();
    }
};
#endif