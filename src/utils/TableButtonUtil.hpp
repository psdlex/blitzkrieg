#pragma once
#ifndef TABLE_BUTTON_UTIL_HPP
#define TABLE_BUTTON_UTIL_HPP

#include "Geode/Geode.hpp"
#include "../defines/TableButton.hpp"

using namespace geode::prelude;

class TableButtonUtil {
public:
    static CircleButtonSprite* createTableButtonSprite() {
        return CircleButtonSprite::createWithSpriteFrameName(BLITZKRIEG_SPRITE_ID, 0.85f, CircleBaseColor::Blue, CircleBaseSize::Tiny);
    }

private:
    TableButtonUtil() {};
};

#endif // TABLE_BUTTON_UTIL_HPP