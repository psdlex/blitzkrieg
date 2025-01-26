#pragma once
#ifndef PROGRESS_DESCRIPTOR_NODE_HPP
#define PROGRESS_DESCRIPTOR_NODE_HPP

#include <Geode/Geode.hpp>
#include "../../utils/SettingsUtil.hpp"

using namespace geode::prelude;

typedef SettingsUtils::ProgressionSettingsFlags PSFlags;

class ProgressDescriptorNode : public CCNode
{
protected:
    CCMenu* m_rootMenu;

private:
    bool init(float width);
    std::vector<std::pair<std::string, float>> createVector(PSFlags flags);

public:
    static ProgressDescriptorNode* create(float width);
};

#endif // PROGRESS_DESCRIPTOR_NODE_HPP