#pragma once
#ifndef VECTOR_UTIL_HPP
#define VECTOR_UTIL_HPP

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class VectorUtil {
public:
    template <typename T>
    static std::optional<size_t> indexOf(const std::vector<T>& vec, const T& value) {
        auto it = std::find(vec.begin(), vec.end(), value);
        if (it != vec.end()) {
            return std::distance(vec.begin(), it); // Return the index
        }
        return std::nullopt; // Return std::nullopt if not found
    }

private:
    VectorUtil() {};
};

#endif // VECTOR_UTIL_HPP