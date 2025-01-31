#pragma once
#ifndef PROGRESS_NODE_HPP
#define PROGRESS_NODE_HPP

#define INIT_TYPES ProgressInfo*,float,const\ std::vector<float>*

#include "Geode/Geode.hpp"
#include "../../managers/LogManager.hpp"
#include "../../ui/CheckableNode.hpp"
#include "../../ui/NodeInitiator.hpp"
#include "../../objects/ProgressInfo.hpp"

using namespace geode::prelude;

class ProgressNode : public CheckableNode<ProgressNode*, bool>, public NodeInitiator<ProgressNode, INIT_TYPES> {
    friend class NodeInitiator<ProgressNode, INIT_TYPES>;

protected:
    // data
    ProgressInfo* m_progressInfo;

    // menus
    CCMenu* m_menu;

    // descriptions
    CCLabelBMFont* m_progressLabel;
    CCLabelBMFont* m_bestRunLabel;
    CCLabelBMFont* m_passAmountLabel;
    CCLabelBMFont* m_attemptsLabel;

    // other
    CCMenuItemToggler* m_passToggler;

protected:
    bool init(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses);
    void setupMenu();
    void setupDescriptions(const std::vector<float>* descriptorsPoses);

    CCLabelBMFont* createProgressLabel();
    CCLabelBMFont* createBestRunLabel();
    CCLabelBMFont* createPassAmountLabel();
    CCLabelBMFont* createAttemptsLabel();
    CCMenuItemToggler* createPassToggler();
    
    void onCheckImpl(CCObject* sender) override;

public:
    void setEnabled(bool isTrue);
    void setPassed(bool isTrue);
    void updateDescriptionValues();

    bool isChecked();
};

#endif // PROGRESS_NODE_HPP