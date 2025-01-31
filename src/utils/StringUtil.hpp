#pragma once
#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <set>
#include <string>

class StringUtil {
public:
    template <typename T>
    static std::string joinVector(const std::vector<T>& vec, const std::string& separator) {
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