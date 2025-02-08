#pragma once
#ifndef LEVEL_PROGRESSION_HPP
#define LEVEL_PROGRESSION_HPP

#include <vector>
#include "../objects/ProgressionStage.hpp"

struct LevelProgression {
    std::vector<ProgressionStage> m_stages;
    std::vector<int> m_startPosPercents;

    void reset() {
        for (auto& stage : m_stages) {
            stage.reset();
        }
    }

    ProgressionStage* getActiveStage() {
        for (auto& stage : m_stages) {
            if (stage.m_isActive) {
                return &stage;
            }
        }

        return nullptr;
    }
};

#endif // LEVEL_PROGRESSION_HPP