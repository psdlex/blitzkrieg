#pragma once
#ifndef PROGRESSION_STAGE_HPP
#define PROGRESSION_STAGE_HPP

#include "../objects/ProgressInfo.hpp"
#include <vector>

struct ProgressionStage {
    bool m_isActive = false;
    bool m_isPassed = false;
    bool m_isAvailable = false;
    
    uint32_t m_stage;
    std::vector<ProgressInfo> m_progresses;

    void reset()  {
        m_isPassed = false;
        m_isActive = m_isAvailable = (m_stage == 1);

        for (auto& progress : m_progresses) {
            progress.reset();
        }
    }

    ProgressInfo* getProgressInfoByStartingPercent(int percent) {
        for (auto& progress : m_progresses) {
            if (progress.m_fromPercent == percent) {
                return &progress;
            }
        }

        return nullptr;
    }

    bool allProgressesPassed() {
        bool all = true;
        for (auto& progress : m_progresses) {
            all &= progress.m_isPassed;
        }

        return all;
    }
};

#endif // PROGRESSION_STAGE_HPP