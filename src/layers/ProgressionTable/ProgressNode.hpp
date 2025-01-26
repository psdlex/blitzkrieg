#pragma once
#ifndef PROGRESS_NODE_HPP
#define PROGRESS_NODE_HPP

#include "Geode/Geode.hpp"
#include "../../objects/ProgressInfo.hpp"

using namespace geode::prelude;

class ProgressNode : public CCNode
{
protected:
    // data
    ProgressInfo* m_progressInfo;
    float m_itemScaledWidth = 0;

    // menus
    CCMenu* m_menu;

    // other
    CCMenuItemToggler* m_isChecked;

protected:
    bool init(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses);

    CCLabelBMFont* createProgressLabel();
    CCLabelBMFont* createBestRunLabel();
    CCLabelBMFont* createPassAmountLabel();
    CCLabelBMFont* createAttemptsLabel();
    CCMenuItemToggler* createPassToggler();

    void onProgressCheck(CCObject* object);

public:
    static ProgressNode* create(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses);
};

#endif // PROGRESS_NODE_HPP