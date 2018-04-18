#include "cache_def.h"

template <>
std::string getKeyEqualValue(bool &t)
{
    return t ? " = 1" : " = 0";
}

template <>
std::string getStringValue(bool &t)
{
    return t ? "1" : "0";
}
