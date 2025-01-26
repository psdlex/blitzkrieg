// #define ╔
// #define ║
// #define ═
// #define ╚
// #define ╗ {
// #define ╝ }
// #define LOAAAADDEEEEDDDD $on_mod(LOADED)

// ╔══════════════ LOAAAADDEEEEDDDD ══════════════╗
// ║ ╔══════════════════════════════════════════╗ ║
// ║ ║    ╔══════════ if (true) ═══════════╗    ║ ║
// ║ ║    ║ log::info("BLITZKRIEG LOADED") ║    ║ ║
// ║ ║    ╚════════════════════════════════╝    ║ ║
// ║ ╚══════════════════════════════════════════╝ ║
// ╚══════════════════════════════════════════════╝

#include <Geode/Geode.hpp>
#include "managers/LevelProgressionManager.hpp"

using namespace geode::prelude;

$on_mod(Loaded)
{
    auto lpmInit = LevelProgressionManager::init();
    auto puInit = PathUtil::init();
    if (!lpmInit.isOk()) {
        log::error("ERROR WHILE INITIALIZING: {0}", lpmInit.unwrapErr());
    }
    else if (!puInit.isOk()) {
        log::error("ERROR WHILE INITIALIZING: {0}", puInit.unwrapErr());
    }
    else {
        log::info("BLITZKRIEG FULLY LOADED");
    }
}