#pragma once
#ifndef PROGRESSION_INFO_HPP
#define PROGRESSION_INFO_HPP

#include "../objects/ProgressBestRun.hpp"

struct ProgressInfo {
    double m_fromPercent;
    double m_toPercent;

    uint32_t m_passAmount;
    uint32_t m_attemptsToPass;
    ProgressBestRun m_bestRun;
};

#endif // PROGRESSION_INFO_HPP