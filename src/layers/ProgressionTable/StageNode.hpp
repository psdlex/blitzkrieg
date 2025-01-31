#pragma once
#ifndef PROGRESSION_STAGE_NODE_HPP
#define PROGRESSION_STAGE_NODE_HPP

#include "Geode/Geode.hpp"
#include "../../managers/LogManager.hpp"
#include "../../ui/CheckableNode.hpp"
#include "../../ui/NodeInitiator.hpp"
#include "../../objects/ProgressionStage.hpp"
#include "ProgressNode.hpp"

using namespace geode::prelude;

class StageNode : public CheckableNode<StageNode*, bool>, public NodeInitiator<StageNode, ProgressionStage*, std::function<void()>, float> {
    friend class NodeInitiator<StageNode, ProgressionStage*, std::function<void()>, float>;

protected:
    // data
    std::function<void()> m_saveFunc;
    ProgressionStage* m_progressionStage;
    
    float m_menuWidth;
    float m_menuHeight;

    // menus
    CCMenu* m_rootMenu;
    CCMenu* m_stageMenu;
    CCMenu* m_progressInfosMenu;

    // other
    CCScale9Sprite* m_passedLayer;
    
    CCLayerColor* m_blockingLayer;
    CCLayerColor* m_background;
    CCLabelBMFont* m_stageIndexLabel;
    CCMenuItemToggler* m_passToggler;

    std::vector<ProgressNode*> m_progressNodes;

protected:
    bool init(ProgressionStage* stage, std::function<void()> saveFunc, float width);

    void setupMenus();
    void setupBasics(float width);
    void setupStage();
    void setupProgresses();

    void onCheckImpl(CCObject* sender) override;
    void onProgressCheck(ProgressNode* node, bool checked);

    bool areAllNodesChecked();

public:
    void setEnabled(bool isTrue);
    void setPassed(bool isTrue, bool setToChildren, bool reverse = true);
    void setActive(bool isTrue);

    uint32_t getStageIndex() { return m_progressionStage->m_stage - 1; }
};

#endif // PROGRESSION_STAGE_NODE_HPP