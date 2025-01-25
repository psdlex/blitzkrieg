#pragma once
#ifndef PROGRESSION_STAGE_UTIL_HPP
#define PROGRESSION_STAGE_UTIL_HPP

#include <set>
#include <algorithm>
#include "../objects/LevelProgression.hpp"
#include "../abstraction/SingletonBase.hpp"

class ProgressionStageUtil : public SingletonBase<ProgressionStageUtil>
{
    friend class SingletonBase<ProgressionStageUtil>;

public:
    std::vector<ProgressionStage> createStages(const std::set<uint32_t>* percents) {
        std::vector<int> sortedPercents(percents->begin(), percents->end());
        std::sort(sortedPercents.begin(), sortedPercents.end());

        if (sortedPercents.front() != 0) {
            sortedPercents.insert(sortedPercents.begin(), 0);
        }
        
        if (sortedPercents.back() != 100) {
            sortedPercents.push_back(100);
        }
        
        size_t stageAmount = sortedPercents.size() - 1;
        std::vector<ProgressionStage> result;

        for (size_t stageIndex = 0; stageIndex < stageAmount; stageIndex++) {
            ProgressionStage stage;
            stage.m_stage = stageIndex + 1;

            for (size_t i = sortedPercents.size() - stageIndex - 2; i != static_cast<size_t>(-1); --i) {
                ProgressInfo info;
                info.m_fromPercent = sortedPercents[i];
                info.m_toPercent = sortedPercents[i + stageIndex + 1];
                info.m_passAmount = 0;
                stage.m_progresses.push_back(info);
            }

            result.push_back(stage);
        }

        return result;
    }

private:
    ProgressionStageUtil() {};
};

#endif // PROGRESSION_STAGE_UTIL_HPP