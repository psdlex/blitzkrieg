#pragma once
#ifndef PROGRESSION_STAGE_HPP
#define PROGRESSION_STAGE_HPP

#include "../objects/ProgressInfo.hpp"
#include <vector>

struct ProgressionStage
{
    bool m_isActive;
    bool m_checked;
    
    uint32_t m_stage;
    std::vector<ProgressInfo> m_progresses;
};

#endif // PROGRESSION_STAGE_HPP