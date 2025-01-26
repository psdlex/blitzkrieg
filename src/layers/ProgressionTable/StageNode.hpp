#pragma once
#ifndef PROGRESSION_STAGE_NODE_HPP
#define PROGRESSION_STAGE_NODE_HPP

#include "Geode/Geode.hpp"
#include "../../objects/ProgressionStage.hpp"
#include "../ProgressionTable/ProgressNode.hpp"

using namespace geode::prelude;

class StageNode : public CCNode
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
    CCLayerColor* m_background;
    CCLabelBMFont* m_stageIndexLabel;
    CCMenuItemToggler* m_stageToggler;

    std::vector<ProgressNode*> m_progressNodes;

protected:
    bool init(ProgressionStage* stage, float width);

    void setupMenus();
    void setupBasics(float width);
    void setupStage();
    void setupProgresses();

    void onStageCheck(CCObject* sender);    

public:
    static StageNode* create(ProgressionStage* stage, float width);
};

#endif // PROGRESSION_STAGE_NODE_HPP