#pragma once
#ifndef LEVEL_PROGRESSION_HPP
#define LEVEL_PROGRESSION_HPP

#include <vector>
#include "../objects/ProgressionStage.hpp"

struct LevelProgression {
    std::vector<ProgressionStage> m_stages;
    std::vector<int> m_startPosPercents;
};

#endif // LEVEL_PROGRESSION_HPP