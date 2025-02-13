#pragma once
#ifndef SETTINGS_CATEGORIES_HPP
#define SETTINGS_CATEGORIES_HPP

#include <Geode/Geode.hpp>
#include "../../include/EnumBitmask.hpp"

using namespace geode::prelude;

enum class ProgressionSettings {
    None = 0,
    ShowBestRun = 1,
    ShowAttempts = 2,
    ShowPassAmount = 4,
    EnableAutoChecker = 8
};

struct SfxSettings {
    bool m_enabled;
    bool m_useCustom;
    float m_volume;
    
    std::filesystem::path m_progressSfxPath;
    std::filesystem::path m_stageSfxPath;
};

enum class DebugSettings {
    Disabled = 0,
    ErrorsAndWarns = 1,
    Descriptive = 2
};

ENUM_BITWISE_OPERATORS(ProgressionSettings)
ENUM_BITWISE_OPERATORS(DebugSettings)

typedef ProgressionSettings PSFlags;
typedef DebugSettings DSFlags;

#endif // SETTINGS_CATEGORIES_HPP