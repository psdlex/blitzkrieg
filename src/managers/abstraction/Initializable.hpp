#pragma once
#ifndef INITIABLE_HPP
#define INITIABLE_HPP

#include "Geode/Result.hpp"
#include <vector>

class Initializable
{
public:
    virtual Result<> init() = 0;
};

#endif // INITIABLE_HPP