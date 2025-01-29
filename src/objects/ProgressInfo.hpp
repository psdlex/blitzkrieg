#pragma once
#ifndef PROGRESSION_INFO_HPP
#define PROGRESSION_INFO_HPP

#include "../objects/ProgressBestRun.hpp"

struct ProgressInfo {
    bool m_isPassed = false;
    uint32_t m_fromPercent = 0;
    uint32_t m_toPercent = 0;
    uint32_t m_passAmount = 0;
    uint32_t m_attemptsToPass = 0;

    ProgressBestRun m_bestRun = {};

    void reset()
    {
        m_isPassed = false;
        m_passAmount = 0;
        m_attemptsToPass = 0;
        m_bestRun.reset();
    }
};

#endif // PROGRESSION_INFO_HPP