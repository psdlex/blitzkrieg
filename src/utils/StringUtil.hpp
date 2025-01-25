#pragma once
#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <set>
#include <string>
#include "../abstraction/SingletonBase.hpp"

class StringUtil : public SingletonBase<StringUtil>
{
    friend class SingletonBase<StringUtil>;

public:
    template <typename T>
    std::string joinVector(const std::vector<T>& vec, const std::string& separator) {
        if (vec.empty()) {
            return "";
        }

        std::ostringstream oss;
        auto it = vec.begin();

        oss << *it;
        ++it;

        for (; it != vec.end(); ++it) {
            oss << separator << *it;
        }

        return oss.str();
    }


private:
    StringUtil() {};
};

#endif // STRING_UTIL_HPP