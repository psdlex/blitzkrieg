#include "../ProgressionTable/StageNode.hpp"
#include "../ProgressionTable/ProgressDescriptorNode.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/ProgressionStageNode.hpp"
#include "../../defines/Fonts.hpp"

bool StageNode::init(ProgressionStage* stage, matjson::Value* settings, float width)
{
    if (!CCNode::init()) {
        return false;
    }

    m_settings = settings;
    m_progressionStage = stage;
    m_menuWidth = width;

    setupBasics(width);
    setupMenus();
    setupStage();
    setupProgresses();
    return true;
}

void StageNode::setupBasics(float width)
{
    // data (size() + 1 for progress descriptor (progress, atts, pass amount, etc...))
    m_menuHeight = ((m_progressionStage->m_progresses.size() + 1) * (PROGRESS_HEIGHT + PROGRESS_MARGIN) - PROGRESS_MARGIN) + (STAGE_MENU_HEIGHT + STAGE_MENU_MARGIN) + (ROOT_MENU_MARGIN * 2);

    // content size
    setContentSize({ m_menuWidth, m_menuHeight });

    // background
    m_background = CCLayerColor::create(STAGE_BACKGROUND_CCC4);
    m_background->setAnchorPoint({ 0.5, 0.5 });
    m_background->ignoreAnchorPointForPosition(false);
    m_background->setContentSize(getContentSize());    
    m_background->setID("stage-bg");

    this->addChildAtPosition(m_background, Anchor::Center);
}

void StageNode::setupMenus()
{
    // root menu
    m_rootMenu = CCMenu::create();
    m_rootMenu->setAnchorPoint({ 0, 1 });
    m_rootMenu->ignoreAnchorPointForPosition(false);
    m_rootMenu->setContentSize({ m_menuWidth - ROOT_MENU_MARGIN * 2, m_menuHeight - ROOT_MENU_MARGIN * 2 });
    m_rootMenu->setID("stage-root-menu");
    m_rootMenu->setLayout(ColumnLayout::create()
                          ->setAxisReverse(true)
                          ->setGap(STAGE_MENU_MARGIN)
                          ->setAxisAlignment(AxisAlignment::End));

    this->addChildAtPosition(m_rootMenu, Anchor::TopLeft, { ROOT_MENU_MARGIN, -ROOT_MENU_MARGIN });

    // updating width
    m_menuWidth = m_rootMenu->getContentWidth();

    // stage menu
    m_stageMenu = CCMenu::create();
    m_stageMenu->setAnchorPoint({ 0, 1 });
    m_stageMenu->ignoreAnchorPointForPosition(false);
    m_stageMenu->setContentSize({ m_menuWidth, STAGE_MENU_HEIGHT });
    m_stageMenu->setID("stage-base-menu");

    m_rootMenu->addChild(m_stageMenu);

    // progresses menu
    m_progressInfosMenu = CCMenu::create();
    m_progressInfosMenu->ignoreAnchorPointForPosition(false);
    m_progressInfosMenu->setAnchorPoint({ 0, 1 });
    m_progressInfosMenu->setContentSize({ m_menuWidth, m_rootMenu->getContentHeight() - STAGE_MENU_HEIGHT - STAGE_MENU_MARGIN });
    m_progressInfosMenu->setID("stage-progress-menu");
    m_progressInfosMenu->setLayout(ColumnLayout::create()
                                   ->setGap(PROGRESS_MARGIN)
                                   ->setAxisReverse(true)
                                   ->setAxisAlignment(AxisAlignment::End));

    m_rootMenu->addChild(m_progressInfosMenu);

    // updating
    m_rootMenu->updateLayout();
}

void StageNode::setupStage()
{
    // stage info
    m_stageIndexLabel = CCLabelBMFont::create(std::format("Stage {0}", m_progressionStage->m_stage).c_str(), BIGFONT);
    m_stageIndexLabel->setScale(0.5);
    m_stageIndexLabel->ignoreAnchorPointForPosition(false);
    m_stageIndexLabel->setAnchorPoint({ 0, 0.5 });
    m_stageIndexLabel->setID("stage-index");

    m_stageMenu->addChildAtPosition(m_stageIndexLabel, Anchor::Left);

    // stage toggler
    m_stageToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(StageNode::onStageCheck), 1.f);
    m_stageToggler->setScale(0.5);
    m_stageToggler->setAnchorPoint({ 1, 0.5 });
    m_stageToggler->ignoreAnchorPointForPosition(false);
    m_stageToggler->setID("stage-toggler");

    m_stageMenu->addChildAtPosition(m_stageToggler, Anchor::Right);
}

void StageNode::setupProgresses()
{
    auto progressDescriptor = ProgressDescriptorNode::create(m_settings, m_menuWidth);
    m_progressInfosMenu->addChild(progressDescriptor);

    for (auto& progress : m_progressionStage->m_progresses)
    {
        auto progressNode = ProgressNode::create(&progress, m_settings, m_menuWidth);
        m_progressInfosMenu->addChild(progressNode);
        m_progressNodes.push_back(progressNode);
    }

    m_progressInfosMenu->updateLayout();
}

void StageNode::onStageCheck(CCObject* sender)
{
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    if (!toggler) {
        return;
    }

    m_progressionStage->m_checked = !toggler->m_toggled; // reversing cuz yeah, it takes the value before changing it
}

StageNode* StageNode::create(ProgressionStage* stage, matjson::Value* settings, float width)
{
    auto ret = new StageNode();
    if (ret->init(stage, settings, width)) {
        ret->autorelease();  
        return ret;
    }

    delete ret;
    return nullptr;
}