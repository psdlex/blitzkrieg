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
    matjson::Value* m_settings;
    ProgressInfo* m_progressInfo;

    // menus
    CCMenu* m_menu;

    // other
    CCLabelBMFont* m_fromPercentLabel;
    CCLabelBMFont* m_toPercentLabel;
    CCLabelBMFont* m_passAmount;

    CCMenuItemToggler* m_isChecked;

protected:
    bool init(ProgressInfo* progressInfo, matjson::Value* settings, float width);

public:
    static ProgressNode* create(ProgressInfo* progressInfo, matjson::Value* settings, float width);
};

#endif // PROGRESS_NODE_HPP