#pragma once
#ifndef SETTINGS_CATEGORIES_HPP
#define SETTINGS_CATEGORIES_HPP

#include "../../include/EnumBitmask.hpp"

enum class ProgressionSettings
{
    None = 0,
    ShowBestRun = 1,
    ShowAttempts = 2,
    ShowPassAmount = 4,
    EnableAutoChecker = 8
};

enum class DebugSettings
{
    None = 0,
    ShowDebug = 1,
    ShowInfo = 2,
    ShowWarns = 4,
    ShowErrors = 8
};

ENUM_BITWISE_OPERATORS(ProgressionSettings)
ENUM_BITWISE_OPERATORS(DebugSettings)

typedef ProgressionSettings PSFlags;
typedef DebugSettings DSFlags;

#endif // SETTINGS_CATEGORIES_HPP