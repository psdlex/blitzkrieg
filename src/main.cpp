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
    auto init = LevelProgressionManager::init();
    if (!init.isOk())
    {
        log::error("ERROR WHILE INITIALIZING: {0}", init.unwrapErr());
        return;
    }

    log::info("BLITZKRIEG FULLY LOADED");
}