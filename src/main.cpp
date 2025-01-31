#include <Geode/Geode.hpp>
#include "managers/LevelProgressionManager.hpp"
#include "managers/LogManager.hpp"
#include "managers/SettingsManager.hpp"
#include "managers/PathManager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    std::array<Initializable*, 4> initializables = {
        managers::PathManager::get(),
        managers::SettingsManager::get(),
        managers::LogManager::get(),
        managers::LevelProgressionManager::get()
    };

    for (auto& initializable : initializables) {
        auto result = initializable->init();

        if (!result.isOk()) {
            log::error("Error while initializing: {}", result.unwrapErr());
        }
    }

    log::info("Blitzkrieg initialized");
}