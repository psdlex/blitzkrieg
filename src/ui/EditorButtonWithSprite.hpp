#pragma once
#ifndef EDITOR_BUTTON_WITH_SPRITE_HPP
#define EDITOR_BUTTON_WITH_SPRITE_HPP

#define INITARGS cocos2d::CCNode*,EditorBaseColor,float,float

#include <Geode/Geode.hpp>
#include "NodeInitiator.hpp"

using namespace geode::prelude;

class EditorButtonWithSprite : public BasedButtonSprite, public NodeInitiator<EditorButtonWithSprite, INITARGS> {
    friend class NodeInitiator<EditorButtonWithSprite, INITARGS>;

protected:
    bool init(cocos2d::CCNode* onTop, EditorBaseColor color, float topScale, float buttonHeight);
};

#endif // EDITOR_BUTTON_WITH_SPRITE_HPP