#pragma once
#ifndef LEVEL_PROGRESSION_HPP
#define LEVEL_PROGRESSION_HPP

#include <vector>
#include "../objects/ProgressionStage.hpp"
// #include "../utils/VectorUtil.hpp"

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

    void setStagePassed(ProgressionStage* stage) {
        // auto result = VectorUtil::indexOf(m_stages, *stage);
        // if (result == std::nullopt) {
        //     return;
        // }

        auto index = 1; // = result.value();

        stage->m_isPassed = true;
        stage->m_isActive = false;

        // next stage
        if (++index < m_stages.size()) {
            auto nextStage = m_stages[index];
            nextStage.m_isActive = true;
            nextStage.m_isAvailable = true;
        }
    }

    constexpr bool operator==(const LevelProgression& left, const LevelProgression& right) const {
        return left.m_stages == right.m_stages;
    }

    // bool operator!=(const LevelProgression& other) const {
    //     return !(*this == other);
    // }
};

#endif // LEVEL_PROGRESSION_HPP