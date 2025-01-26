#pragma once
#ifndef PROGRESSION_INFO_HPP
#define PROGRESSION_INFO_HPP

#include "../objects/ProgressBestRun.hpp"

struct ProgressInfo {
    uint32_t m_fromPercent;
    uint32_t m_toPercent;
    uint32_t m_passAmount = 0;
    uint32_t m_attemptsToPass = 0;
    ProgressBestRun m_bestRun = {};
};

#endif // PROGRESSION_INFO_HPP