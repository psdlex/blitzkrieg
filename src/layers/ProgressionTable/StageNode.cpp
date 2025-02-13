#include "StageNode.hpp"
#include "ProgressDescriptorNode.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/ProgressionStageNode.hpp"
#include "../../defines/Fonts.hpp"

bool StageNode::init(ProgressionStage* stage, float width) {
    LMDEBUG("Initializing StageNode");

    if (!CCNode::init()) {
        LMERROR("Failed to initialize StageNode (CCNode init failed)");
        return false;
    }

    m_progressionStage = stage;
    m_menuWidth = width;

    setupBasics(width);
    setupMenus();
    setupStage();
    setupProgresses();

    setActive(stage->m_isActive);
    setPassed(stage->m_isPassed, false, false);
    setEnabled(stage->m_isAvailable);

    registerCheckable(m_passToggler);
    return true;
}

void StageNode::setupBasics(float width) {
    // data (stage menu) + (size() + 1 for progress descriptor (progress, atts, pass amount, etc...))
    m_menuHeight = ((m_progressionStage->m_progresses.size() + 1) * (PROGRESS_HEIGHT + PROGRESS_MARGIN) - PROGRESS_MARGIN) + 
                                                                    (STAGE_MENU_HEIGHT + STAGE_MENU_MARGIN) +
                                                                    (ROOT_MENU_MARGIN * 2);

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

void StageNode::setupMenus() {
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
    m_stageMenu->setAnchorPoint({ 0.5, 1 });
    m_stageMenu->ignoreAnchorPointForPosition(false);
    m_stageMenu->setContentSize({ m_menuWidth - 8, STAGE_MENU_HEIGHT });
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

    // blocking layer
    m_blockingLayer = CCLayerColor::create(ccc4(0,0,0,90));
    m_blockingLayer->ignoreAnchorPointForPosition(false);
    m_blockingLayer->setVisible(false);
    m_blockingLayer->setContentSize(getContentSize());
    m_blockingLayer->setAnchorPoint({ 0.5, 0.5 });

    this->addChildAtPosition(m_blockingLayer, Anchor::Center);

    // passed layer
    m_passedLayer = CCScale9Sprite::createWithSpriteFrameName("passed-outline.png"_spr);
    m_passedLayer->ignoreAnchorPointForPosition(false);
    m_passedLayer->setVisible(false);
    m_passedLayer->setContentSize(getContentSize());
    m_passedLayer->setAnchorPoint({ 0.5, 0.5 });

    this->addChildAtPosition(m_passedLayer, Anchor::Center);

    // updating
    m_rootMenu->updateLayout();
}

void StageNode::setupStage() {
    // stage info
    m_stageIndexLabel = CCLabelBMFont::create(std::format("Stage {0}", m_progressionStage->m_stage).c_str(), BIGFONT);
    m_stageIndexLabel->setScale(0.5);
    m_stageIndexLabel->ignoreAnchorPointForPosition(false);
    m_stageIndexLabel->setAnchorPoint({ 0, 0.5 });
    m_stageIndexLabel->setID("stage-index");

    m_stageMenu->addChildAtPosition(m_stageIndexLabel, Anchor::Left);

    // stage toggler
    m_passToggler = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 1.f);
    m_passToggler->setScale(0.5);
    m_passToggler->setAnchorPoint({ 1, 0.5 });
    m_passToggler->ignoreAnchorPointForPosition(false);
    m_passToggler->setID("stage-toggler");
    m_passToggler->toggle(m_progressionStage->m_isPassed);

    m_stageMenu->addChildAtPosition(m_passToggler, Anchor::Right);
}

void StageNode::setupProgresses() {
    auto progressDescriptor = ProgressDescriptorNode::createNode(m_menuWidth);
    m_progressInfosMenu->addChild(progressDescriptor);

    for (auto& progress : m_progressionStage->m_progresses) {
        auto progressNode = ProgressNode::createNode(&progress, m_menuWidth, progressDescriptor->getDescriptorsPositions());
        progressNode->onCheck([&](ProgressNode* node, bool checked) { onProgressCheck(node, checked); });

        m_progressInfosMenu->addChild(progressNode);
        m_progressNodes.push_back(progressNode);
    }

    m_progressInfosMenu->updateLayout();
}


void StageNode::onCheckImpl(CCObject* sender) {
    LMDEBUG("Toggler clicker event raised for StageNode");
    
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    if (!toggler) {
        return;
    }

    bool toggled = !toggler->isToggled();
    if (toggled == true && !m_progressionStage->allProgressesPassed()) {
        toggler->m_toggled = !(false); // reverse again
        return;
    }

    m_checkFunc(this, toggled);
}

void StageNode::onProgressCheck(ProgressNode* node, bool checked) {
    // handling progress check from stage class because its desicive for the stage as well
    LMDEBUG("Handling progress check for stage: {}", m_progressionStage->m_stage);

    // cant uncheck progress while stage is checked
    if (checked == false && m_progressionStage->m_isPassed) {
        node->setPassed(!(true)); // reverse
    }
}


void StageNode::setEnabled(bool isTrue) {
    LMDEBUG("Changing state 'enabled' for stage {}: {}", m_progressionStage->m_stage, isTrue);

    m_progressionStage->m_isAvailable = isTrue;
    m_blockingLayer->setVisible(!isTrue);
    m_stageMenu->setEnabled(isTrue);

    for (auto& node : m_progressNodes) {
        node->setEnabled(isTrue);
    }
}

void StageNode::setPassed(bool isTrue, bool setToChildren, bool reverse) {
    LMDEBUG("Changing state 'passed' for stage {}: {}", m_progressionStage->m_stage, isTrue);

    m_progressionStage->m_isPassed = isTrue;
    m_passToggler->toggle(reverse ? !isTrue : isTrue);
    m_passedLayer->setVisible(isTrue);
    
    if (setToChildren == false) {
        return;
    }
    
    for (auto& node : m_progressNodes) {
        node->setPassed(reverse ? !isTrue : isTrue);
    }
}

void StageNode::setActive(bool isTrue) {
    LMDEBUG("Changing state 'active' for stage {}: {}", m_progressionStage->m_stage, isTrue);
    m_progressionStage->m_isActive = isTrue;
}