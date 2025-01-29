#pragma once
#ifndef PROGRESS_DESCRIPTOR_NODE_HPP
#define PROGRESS_DESCRIPTOR_NODE_HPP

#include <Geode/Geode.hpp>
#include "../../managers/SettingsManager.hpp"

using namespace geode::prelude;

class ProgressDescriptorNode : public CCNode
{
protected:
    CCMenu* m_rootMenu;
    std::vector<float> _descriptorsPositionsX;

private:
    bool init(float width);
    std::vector<std::pair<std::string, float>> createVector(PSFlags flags);

public:
    static ProgressDescriptorNode* create(float width);

    std::vector<float>* getDescriptorsPositions() { return &_descriptorsPositionsX; }
};

#endif // PROGRESS_DESCRIPTOR_NODE_HPP