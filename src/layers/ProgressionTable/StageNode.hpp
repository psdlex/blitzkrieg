#pragma once
#ifndef PROGRESSION_STAGE_NODE_HPP
#define PROGRESSION_STAGE_NODE_HPP

#include "Geode/Geode.hpp"
#include "../../objects/ProgressionStage.hpp"
#include "../ProgressionTable/ProgressNode.hpp"
#include "../../ui/CheckableNode.hpp"

using namespace geode::prelude;

class StageNode : public CheckableNode<StageNode*, bool>
{
protected:
    // data
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
    bool init(ProgressionStage* stage, float width);

    void setupMenus();
    void setupBasics(float width);
    void setupStage();
    void setupProgresses();

    void onCheckImpl(CCObject* sender) override;
    void onProgressCheck(ProgressNode* node, bool checked);

    bool allNodesChecked();

public:
    static StageNode* create(ProgressionStage* stage, float width);

    void setEnabled(bool isTrue);
    void setPassed(bool isTrue, bool setToChildren, bool reverse = true);
    void setActive(bool isTrue);

    uint32_t getStageIndex() { return m_progressionStage->m_stage - 1; }
};

#endif // PROGRESSION_STAGE_NODE_HPP