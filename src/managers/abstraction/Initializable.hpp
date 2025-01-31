#pragma once
#ifndef INITIABLE_HPP
#define INITIABLE_HPP

#include "Geode/Geode.hpp"

using namespace geode::prelude;

class Initializable {
public:
    virtual Result<> init() = 0;
};

#endif // INITIABLE_HPP