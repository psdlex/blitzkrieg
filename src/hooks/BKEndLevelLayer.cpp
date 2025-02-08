#include "BKEndLevelLayer.hpp"
#include "../managers/LogManager.hpp"
#include "../managers/LevelProgressionManager.hpp"
#include "../layers/ProgressionTablePopup.hpp"
#include "../utils/TableButtonUtil.hpp"
#include "../utils/DialogsUtil.hpp"
#include "../defines/EndLevelLayer.hpp"

using namespace managers;

void BKEndLevelLayer::customSetup() {
    EndLevelLayer::customSetup();

    auto hideLayer = getChildByID(END_LAYER_HIDE_LAYER_MENU_ID);
    if (!hideLayer) {
        LMERROR("Child not found: {}", END_LAYER_HIDE_LAYER_MENU_ID);
        return;
    }

    auto level = PlayLayer::get()->m_level;

    LMDEBUG("Trying to get cached progression instance for level '{}'", level->m_levelName);
    auto progression = LevelProgressionManager::get()->getCachedProgression(level);

    if (!progression.isOk()) {
        LMINFO("Cached progression not found for level '{}'", level->m_levelName);
        return;
    }

    m_fields->m_levelProgression = progression.unwrap();

    auto menu = CCMenu::create();
    menu->setPositionX(hideLayer->getPositionX());
    menu->setPositionY(hideLayer->getPositionY() - 25);
    menu->setTouchPriority(INT_MAX);

    auto button = CCMenuItemSpriteExtra::create(
        TableButtonUtil::createTableButtonSprite(),
        this,
        menu_selector(BKEndLevelLayer::onShowProgressionTable)
    );
    
    button->setScale(1.1);
    button->setID(BLITKRIEG_BUTTON_ID);
    menu->addChild(button);

    this->addChild(menu);;
}

void BKEndLevelLayer::onShowProgressionTable(CCObject*) {
    auto progression = m_fields->m_levelProgression;
    if (progression) {
        showProgressionTable(progression);
    }
}

void BKEndLevelLayer::showProgressionTable(LevelProgression* progression) {
    ProgressionTablePopup::create(progression)->show();
}