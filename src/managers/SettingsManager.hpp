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
        #define GetSettingValue Mod::get()->getSettingValue
        #define GetBoolSetting Mod::get()->getSettingValue<bool>
        #define GetStringSetting Mod::get()->getSettingValue<std::string>

        friend class SingletonBase<SettingsManager>;

    private:
        std::vector<std::function<void(DSFlags)>> m_debugSettingsListeners = {};
        std::vector<std::function<void(SfxSettings)>> m_sfxSettingsListeners = {};

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

        SfxSettings getSfxSettings() {
            SfxSettings settings;
            settings.m_enabled = GetBoolSetting(SFX_ENABLED_SETTING);
            settings.m_useCustom = GetBoolSetting(SFX_USE_CUSTOM_SOUNDS_SETTING);
            settings.m_volume = GetSettingValue<int>(SFX_VOLUME_SETTING) / 100.0f;
            settings.m_progressSfxPath = GetSettingValue<std::filesystem::path>(SFX_PROGRESS_PATH_SETTING);
            settings.m_stageSfxPath = GetSettingValue<std::filesystem::path>(SFX_STAGE_PATH_SETTING);
            return settings;
        }

        DSFlags getDebugSettings() {
            auto setting = GetStringSetting(DEBUG_TYPE_SETTING);
            return getDebugSettingsImpl(setting);
        }

        void listenForDebugSettingsChanged(std::function<void(DSFlags)> func) {
            m_debugSettingsListeners.push_back(func);
        }

        void listenForSfxSettingsChanged(std::function<void(SfxSettings)> func) {
            m_sfxSettingsListeners.push_back(func);
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
        // debugs settings
        listenForSettingChangesV3(DEBUG_TYPE_SETTING, [this](std::string newVal) {
            auto result = getDebugSettingsImpl(newVal);
            for (auto& func : m_debugSettingsListeners) {
                func(result);
            }
        });

        // sfx settings
        listenForSettingChangesV3(SFX_ENABLED_SETTING, [this](bool newVal) {
            for (auto& func : m_sfxSettingsListeners) {
                func(getSfxSettings());
            }

        });

        listenForSettingChangesV3<int64_t>(SFX_VOLUME_SETTING, [this](auto newVal) {
            log::warn("sex");
            for (auto& func : m_sfxSettingsListeners) {
                func(getSfxSettings());
            }
        });
    }
};

#endif // SETTINGS_MANAGER_HPP