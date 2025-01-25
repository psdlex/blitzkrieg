#include "../ui/EditorButtonWithSprite.hpp"

bool EditorButtonWithSprite::init(cocos2d::CCNode* onTop, EditorBaseColor color, float topScale, float buttonHeight)
{
    if (!BasedButtonSprite::init(onTop, BaseType::Editor, 0, (int)color))
        return false;

    onTop->ignoreAnchorPointForPosition(false);

    setScale(buttonHeight / getContentHeight());
    setScaledContentSize({ buttonHeight, buttonHeight });
    setTopRelativeScale(topScale);
    return true;
}

EditorButtonWithSprite* EditorButtonWithSprite::create(cocos2d::CCNode* onTop, EditorBaseColor color, float topScale, float buttonHeight)
{
    auto ret = new EditorButtonWithSprite();
    if (ret->init(onTop, color, topScale, buttonHeight)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}