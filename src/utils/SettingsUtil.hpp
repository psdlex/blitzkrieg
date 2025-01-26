#pragma once
#ifndef SETTINGS_UTIL_HPP
#define SETTINGS_UTIL_HPP

#include <Geode/Geode.hpp>
#include "../include/EnumBitmask.hpp"

using namespace geode::prelude;

class SettingsUtils
{
public:
    enum class ProgressionSettingsFlags
    {
        None = 0,
        ShowBestRun = 1,
        ShowAttempts = 2,
        ShowPassAmount = 4,
        EnableAutoChecker = 8
    };
    
    static ProgressionSettingsFlags getProgressionSettings();

private:
    SettingsUtils() {};
};

ENUM_BITWISE_OPERATORS(SettingsUtils::ProgressionSettingsFlags);

inline SettingsUtils::ProgressionSettingsFlags SettingsUtils::getProgressionSettings()
{
    ProgressionSettingsFlags flags = ProgressionSettingsFlags::None;

    //TODO: TEMPORARY DECISION, implement settings manager instead of this dogshit please<3 ! uwu_-
    if (Mod::get()->getSettingValue<bool>("show-best-run")) {
        flags |= ProgressionSettingsFlags::ShowBestRun;
    }

    if (Mod::get()->getSettingValue<bool>("show-attempts-for-progress")) {
        flags |= ProgressionSettingsFlags::ShowAttempts;
    }

    if (Mod::get()->getSettingValue<bool>("show-pass-amount")) {
        flags |= ProgressionSettingsFlags::ShowPassAmount;
    }

    if (Mod::get()->getSettingValue<bool>("enable-auto-checker")) {
        flags |= ProgressionSettingsFlags::EnableAutoChecker;
    }

    return flags;
}

#endif // SETTINGS_UTIL_HPP