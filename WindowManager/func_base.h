#ifndef FUNC_BASE

#include <iostream>
#include <string>

template<typename T>
inline void SafeDelete(T** ptr)
{
    delete *ptr;
    *ptr = NULL;
}
template<typename T>
inline void SafeArrDelete(T** ptr)
{
    delete[] *ptr;
    *ptr = NULL;
}

template <typename T>
char* NumberToPChar(T number)
{
    char* pcharResult = _strdup(std::to_string(number).c_str());
    return pcharResult;
}


class FuncBase
{

};


#endif // !FUNC_BASE







