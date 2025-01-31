#include "BKPauseLayer.hpp"
#include "Geode/modify/PlayLayer.hpp"
#include "../managers/LevelProgressionManager.hpp"
#include "../managers/LogManager.hpp"
#include "../objects/LevelProgression.hpp"
#include "../layers/ProgressionTablePopup.hpp"
#include "../utils/DialogsUtil.hpp"
#include "../defines/PauseLayer.hpp"

using namespace geode::prelude;
using namespace managers;

void BKPauseLayer::customSetup() {
    LMDEBUG("Setting up BKPauseLayer");

	PauseLayer::customSetup();
	createOrLoadProgressTableButton();
}

void BKPauseLayer::createOrLoadProgressTableButton() {
    auto menu = this->getChildByID(RIGHT_BUTTON_MENU_ID);
    if (!menu) {
        LMERROR("Menu '{}' not found.. ?", RIGHT_BUTTON_MENU_ID);
        return;
    }

    LMINFO("Menu '{}' found", RIGHT_BUTTON_MENU_ID);    
    auto button = CCMenuItemSpriteExtra::create(
        CircleButtonSprite::createWithSpriteFrameName(BLITZKRIEG_SPRITE_ID, 0.85, CircleBaseColor::Blue, CircleBaseSize::Tiny),
        this,
        menu_selector(BKPauseLayer::onShowProgressTable)
    );

    button->setID("blitzkrieg-show-table");

    menu->addChildAtPosition(button, Anchor::Center);
    menu->updateLayout();
}

void BKPauseLayer::onShowProgressTable(CCObject*) {
    auto level = PlayLayer::get()->m_level;

    LMDEBUG("Trying to get cached progression instance for level '{}'", level->m_levelName);
    auto progression = LevelProgressionManager::get()->getCachedProgression(PlayLayer::get()->m_level);

    if (progression.isOk()) {
        LMINFO("Cached progression found for level '{}'", level->m_levelName);
        showProgression(progression.unwrap());
        return;
    }

    LMWARN("Cached progression not found for level '{}', suggesting to create one!", level->m_levelName);
    DialogsUtil::showChoice("Create new table",  "<cr>No data found</c>. Would you like to <cb>create a new table</c>?", "No", "Yes", [this](auto result)
    {
        if (result == DialogsUtil::ButtonResult::Right) {
            createNewProgression();
        }
    });
}

void BKPauseLayer::createNewProgression() {
    LMDEBUG("Creating new empty progression...");
    auto progression = LevelProgressionManager::get()->createEmptyProgression(PlayLayer::get()->m_level);

    if (!progression.isOk()) {
        LMERROR("Error while creating table for level, error: {}", progression.unwrapErr());
        DialogsUtil::showAlert("Table Construction", "<cr>Something went horribly wrong</c>.. Couldn't create new table :(", "I forgive you!");
    } else {
        LMDEBUG("Progression created successfully for the current level");
        showProgression(progression.unwrap());
    }
}

void BKPauseLayer::showProgression(LevelProgression* levelProgression) {
    LMDEBUG("Displaying progression table for the current level");
    ProgressionTablePopup::create(levelProgression)->show();
}