#include "../ProgressionTable/ProgressNode.hpp"
#include "../../defines/ProgressNode.hpp"

bool ProgressNode::init(ProgressInfo* progressInfo, float width)
{
    if (!CCNode::init())
        return false;

    m_progressInfo = progressInfo;
    setContentSize({ width, PROGRESS_HEIGHT });

    // MENU
    m_menu = CCMenu::create();
    m_menu->setAnchorPoint({ 0, 1 });
    m_menu->setContentSize(getContentSize());
    m_menu->ignoreAnchorPointForPosition(false);
    m_menu->setID("progress-menu");

    this->addChildAtPosition(m_menu, Anchor::TopLeft);

    // FROM PERCENT
    m_fromPercentLabel = CCLabelBMFont::create(std::to_string(progressInfo->m_fromPercent).c_str(), "bigFont.fnt");
    m_fromPercentLabel->setScale(0.35);
    m_fromPercentLabel->setAnchorPoint({ 0, 0.5 });
    m_fromPercentLabel->ignoreAnchorPointForPosition(false);
    m_fromPercentLabel->setID("progress-from-percent");

    m_menu->addChildAtPosition(m_fromPercentLabel, Anchor::Left);

    return true;
}

ProgressNode* ProgressNode::create(ProgressInfo* progressInfo, float width)
{
    auto ret = new ProgressNode();
    if (ret->init(progressInfo, width)) {
        ret->autorelease();  
        return ret;
    }

    delete ret;
    return nullptr;
}