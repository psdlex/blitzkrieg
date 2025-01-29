#pragma once
#ifndef PROGRESS_BEST_RUN_HPP
#define PROGRESS_BEST_RUN_HPP

struct ProgressBestRun
{
    uint32_t m_fromPercent = 0;
    uint32_t m_toPercent = 0;

    void reset()
    {
        m_fromPercent = m_toPercent = 0;
    }
};

#endif // PROGRESS_BEST_RUN_HPP