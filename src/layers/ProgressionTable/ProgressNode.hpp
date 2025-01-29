#pragma once
#ifndef PROGRESS_NODE_HPP
#define PROGRESS_NODE_HPP

#include "Geode/Geode.hpp"
#include "../../objects/ProgressInfo.hpp"
#include "../../ui/CheckableNode.hpp"

using namespace geode::prelude;

class ProgressNode : public CheckableNode<ProgressNode*, bool>
{
protected:
    // data
    ProgressInfo* m_progressInfo;
    float m_itemScaledWidth = 0;

    // menus
    CCMenu* m_menu;

    // other
    CCMenuItemToggler* m_passToggler;

protected:
    bool init(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses);

    CCLabelBMFont* createProgressLabel();
    CCLabelBMFont* createBestRunLabel();
    CCLabelBMFont* createPassAmountLabel();
    CCLabelBMFont* createAttemptsLabel();
    CCMenuItemToggler* createPassToggler();
    
    void onCheckImpl(CCObject* sender) override;

public:
    static ProgressNode* create(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses);
    void setEnabled(bool isTrue);
    void setPassed(bool isTrue);
    bool isChecked();
};

#endif // PROGRESS_NODE_HPP