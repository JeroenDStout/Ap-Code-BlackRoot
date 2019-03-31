#pragma once

#include <sstream>

template<typename Type>
std::stringstream& operator<<(std::stringstream& ss, const Type& type)
{
    static_cast<std::ostream&>(ss) << type;
    return ss;
}

template<typename Type>
std::stringstream& operator<<(std::stringstream&& ss, const Type& type)
{
    static_cast<std::ostream&>(ss) << type;
    return ss;
}

template<typename Type>
std::stringstream& operator>>(std::stringstream& ss, Type& type)
{
    static_cast<std::istream&>(ss) >> type;
    return ss;
}

template<typename Type>
std::stringstream& operator>>(std::stringstream&& ss, Type& type)
{
    static_cast<std::istream&>(ss) >> type;
    return ss;
}