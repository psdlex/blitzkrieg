#pragma once
#ifndef SETTINGS_MANAGER_HPP
#define SETTINGS_MANAGER_HPP

#include "../abstraction/SingletonBase.hpp"
#include "abstraction/Initializable.hpp"
#include "settings/SettingsCategories.hpp"
#include "../defines/Settings.hpp"

using namespace geode::prelude;

namespace managers {
    class SettingsManager : public SingletonBase<SettingsManager>, public Initializable {
        #define GetBoolSetting Mod::get()->getSettingValue<bool>
        #define GetStringSetting Mod::get()->getSettingValue<std::string>

        friend class SingletonBase<SettingsManager>;

    private:
        std::vector<std::function<void(DSFlags)>> m_debugSettingsChangedListeners = {};

    public:
        Result<> init() override {
            setupSettingsListeners();
            return Ok();
        }

        PSFlags getProgressionSettings() {
            PSFlags flags = PSFlags::None;
            flags |= GetBoolSetting(SHOW_BEST_RUN_SETTING) ? PSFlags::ShowBestRun : PSFlags::None ;
            flags |= GetBoolSetting(SHOW_ATTEMPTS_FOR_PROGRESS_SETTING) ? PSFlags::ShowAttempts : PSFlags::None;
            flags |= GetBoolSetting(SHOW_PASS_AMOUNT_SETTING) ? PSFlags::ShowPassAmount : PSFlags::None;
            flags |= GetBoolSetting(ENABLE_AUTO_CHECKER_SETTING) ? PSFlags::EnableAutoChecker : PSFlags::None;
            return flags;
        }

        DSFlags getDebugSettings() {
            auto setting = GetStringSetting(DEBUG_TYPE_SETTING);
            return getDebugSettingsImpl(setting);
        }

        void listenForDebugSettingsChanged(std::function<void(DSFlags)> func) {
            m_debugSettingsChangedListeners.push_back(func);
        }

    private:
        DSFlags getDebugSettingsImpl(std::string value);
        void setupSettingsListeners();
    };

    inline DSFlags SettingsManager::getDebugSettingsImpl(std::string value) {
        if (value == DEBUG_TYPE_ERRORSANDWARNS) {
            return DSFlags::ErrorsAndWarns;
        } else if (value == DEBUG_TYPE_DESCRIPTIVE) {
            return DSFlags::ErrorsAndWarns | DSFlags::Descriptive;
        }

        return DSFlags::Disabled;
    }

    inline void SettingsManager::setupSettingsListeners() {
        listenForSettingChangesV3(DEBUG_TYPE_SETTING, [this](std::string newVal) {
            auto result = getDebugSettingsImpl(newVal);
            for (auto& func : m_debugSettingsChangedListeners) {
                func(result);
            }
        });
    }
};

#endif // SETTINGS_MANAGER_HPP