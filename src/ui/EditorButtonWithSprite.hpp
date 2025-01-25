#pragma once
#ifndef EDITOR_BUTTON_WITH_SPRITE_HPP
#define EDITOR_BUTTON_WITH_SPRITE_HPP

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class EditorButtonWithSprite : public BasedButtonSprite
{
protected:
    bool init(cocos2d::CCNode* onTop, EditorBaseColor color, float topScale, float buttonHeight);

public:
    static EditorButtonWithSprite* create(cocos2d::CCNode* onTop, EditorBaseColor color, float topScale, float buttonHeight);
};

#endif // EDITOR_BUTTON_WITH_SPRITE_HPP