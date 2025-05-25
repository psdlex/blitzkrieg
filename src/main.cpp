#include <Geode/Geode.hpp>
#include "managers/LevelProgressionManager.hpp"
#include "managers/LogManager.hpp"
#include "managers/SettingsManager.hpp"
#include "managers/PathManager.hpp"
#include "managers/SfxManager.hpp"

using namespace geode::prelude;

void setupNotificationMenu();

$on_mod(Loaded) {
    std::array<Initializable*, 5> initializables = {
        managers::PathManager::get(),
        managers::SettingsManager::get(),
        managers::LogManager::get(),
        managers::LevelProgressionManager::get(),
        managers::SfxManager::get() 
    };

    for (auto& initializable : initializables) {
        auto result = initializable->init();

        if (!result.isOk()) {
            log::error("Error while initializing: {}", result.unwrapErr());
        }
    }

    setupNotificationMenu();
    log::info("Blitzkrieg initialized");
}

void setupNotificationMenu() {
    auto node = CCLayerColor::create(ccc4(255,0,0,100));
    node->ignoreAnchorPointForPosition(false);
    node->setContentSize({50, 50});
    node->setAnchorPoint({ 0.5, 0.5 });

    SceneManager::get()->keepAcrossScenes(node);
}