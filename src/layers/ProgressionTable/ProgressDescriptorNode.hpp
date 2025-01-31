#pragma once
#ifndef PROGRESS_DESCRIPTOR_NODE_HPP
#define PROGRESS_DESCRIPTOR_NODE_HPP

#include <Geode/Geode.hpp>
#include "../../managers/SettingsManager.hpp"
#include "../../managers/LogManager.hpp"
#include "../../ui/NodeInitiator.hpp"

using namespace geode::prelude;

class ProgressDescriptorNode : public CCNode, public NodeInitiator<ProgressDescriptorNode, float> {
    friend class NodeInitiator<ProgressDescriptorNode, float>;

protected:
    CCMenu* m_rootMenu;
    std::vector<float> _descriptorsPositionsX;

protected:
    bool init(float width);

    void setupBasics(float width);
    void setupMenu();
    void setupDescriptors();

    std::vector<std::pair<std::string, float>> createVector(PSFlags flags);

public:
    std::vector<float>* getDescriptorsPositions() { return &_descriptorsPositionsX; }
};

#endif // PROGRESS_DESCRIPTOR_NODE_HPP