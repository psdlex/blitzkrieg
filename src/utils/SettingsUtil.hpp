#pragma once
#ifndef SETTINGS_UTIL_HPP
#define SETTINGS_UTIL_HPP

#include <Geode/Geode.hpp>
#include "../abstraction/SingletonBase.hpp"
#include "../include/EnumBitmask.hpp"

using namespace geode::prelude;

class SettingsUtils : public SingletonBase<SettingsUtils>
{
    friend class SingletonBase<SettingsUtils>;

public:
        enum class ProgressionSettingsFlags
        {
        ShowBestRun = 1,
        ShowAttempts = 2,
        ShowPassAmount = 4
    };

    ProgressionSettingsFlags getProgressionSettings(const matjson::Value* settings)
    {
        ProgressionSettingsFlags flags;
        
        if (settings->get("show-best-run").unwrap().asBool().unwrap()) {
            flags |= ProgressionSettingsFlags::ShowBestRun;
        }

        if (settings->get("show-attempts-for-progress").unwrap().asBool().unwrap()) {
            flags |= ProgressionSettingsFlags::ShowAttempts;
        }

        if (settings->get("show-pass-amount").unwrap().asBool().unwrap()) {
            flags |= ProgressionSettingsFlags::ShowPassAmount;
        }

        return flags;
    }
};

DEFINE_FULL_BITMASK(SettingsUtils::ProgressionSettingsFlags);

#endif // SETTINGS_UTIL_HPP