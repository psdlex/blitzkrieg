#include "Geode/modify/PlayLayer.hpp"
#include "../hooks/BKPauseLayer.hpp"
#include "../objects/LevelProgression.hpp"
#include "../managers/LevelProgressionManager.hpp"
#include "../layers/ProgressionTablePopup.hpp"
#include "../managers/LogManager.hpp"

using namespace geode::prelude;
using namespace managers;

void BKPauseLayer::customSetup()
{
	PauseLayer::customSetup();
	createOrLoadProgressTableButton();
}

void BKPauseLayer::createOrLoadProgressTableButton()
{
    auto menu = this->getChildByID("right-button-menu");
    if (!menu) {
        return;
    }

    auto button = CCMenuItemSpriteExtra::create(
        CircleButtonSprite::createWithSpriteFrameName("blitzkrieg-sprite.png"_spr, 0.85, CircleBaseColor::Blue, CircleBaseSize::Tiny),
        this,
        menu_selector(BKPauseLayer::onShowProgressTable)
    );

    button->setID("blitzkrieg-show-table");
    menu->addChildAtPosition(button, Anchor::Center);
    menu->updateLayout();
}

void BKPauseLayer::onShowProgressTable(CCObject*)
{
    auto progression = LevelProgressionManager::get()->getCachedProgression(PlayLayer::get()->m_level);

    if (progression.isOk()) {
        showProgression(progression.unwrap());
        return;
    }

    createQuickPopup("Create new table", "No data found. Would you like to create a new table?", "No", "Yes", [this](auto, bool isYes)
    {
        if (isYes) {
            createNewProgression();
        }
    });
}

void BKPauseLayer::createNewProgression()
{
    auto progression = LevelProgressionManager::get()->createEmptyProgression(PlayLayer::get()->m_level);

    if (!progression.isOk()) {
        LogManager::get()->error("error while creating table for level: {0}", progression.unwrapErr());
        FLAlertLayer::create("ERROR", "Something went horribly wrong.. Couldnt create new table :(", "I forgive you!")->show();
        return;
    }

    showProgression(progression.unwrap());
}

void BKPauseLayer::showProgression(LevelProgression* levelProgression)
{
    ProgressionTablePopup::create(levelProgression)->show();
}