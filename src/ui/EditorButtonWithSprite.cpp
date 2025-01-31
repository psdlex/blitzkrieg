#include "../ui/EditorButtonWithSprite.hpp"

bool EditorButtonWithSprite::init(cocos2d::CCNode* onTop, EditorBaseColor color, float topScale, float buttonHeight) {
    if (!BasedButtonSprite::init(onTop, BaseType::Editor, 0, (int)color))
        return false;

    onTop->ignoreAnchorPointForPosition(false);

    setScale(buttonHeight / getContentHeight());
    setScaledContentSize({ buttonHeight, buttonHeight });
    setTopRelativeScale(topScale);
    return true;
}