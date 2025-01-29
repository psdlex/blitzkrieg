#pragma once
#ifndef SETTINGS_MANAGER_HPP
#define SETTINGS_MANAGER_HPP

#include "../abstraction/SingletonBase.hpp"
#include "../defines/Settings.hpp"
#include "settings/SettingsCategories.hpp"

using namespace geode::prelude;

namespace managers
{
    class SettingsManager : public SingletonBase<SettingsManager>
    {
        #define GetBoolSetting Mod::get()->getSettingValue<bool>

        friend class SingletonBase<SettingsManager>;

    public:
        PSFlags getProgressionSettings()
        {
            PSFlags flags = PSFlags::None;
            flags |= GetBoolSetting(SHOW_BEST_RUN_SETTING) ? PSFlags::ShowBestRun : PSFlags::None ;
            flags |= GetBoolSetting(SHOW_ATTEMPTS_FOR_PROGRESS_SETTING) ? PSFlags::ShowAttempts : PSFlags::None;
            flags |= GetBoolSetting(SHOW_PASS_AMOUNT_SETTING) ? PSFlags::ShowPassAmount : PSFlags::None;
            flags |= GetBoolSetting(ENABLE_AUTO_CHECKER_SETTING) ? PSFlags::EnableAutoChecker : PSFlags::None;
            return flags;
        }

        DSFlags getDebugSettings()
        {
            DSFlags flags = DSFlags::None;
            flags |= GetBoolSetting(DEBUG_ENABLE_SETTING) ? DSFlags::ShowDebug : DSFlags::None;
            flags |= GetBoolSetting(DEBUG_DISPLAY_INFO) ? DSFlags::ShowInfo : DSFlags::None;
            flags |= GetBoolSetting(DEBUG_DISPLAY_WARNINGS) ? DSFlags::ShowWarns : DSFlags::None;
            flags |= GetBoolSetting(DEBUG_DISPLAY_ERRORS) ? DSFlags::ShowErrors : DSFlags::None;
            return flags;
        }
    };
};

#endif // SETTINGS_MANAGER_HPP