#include <regex>
#include <thread>
#include "ProgressionTablePopup.hpp"
#include "../hooks/BKPlayLayer.hpp"
#include "../managers/LogManager.hpp"
#include "../ui/EditorButtonWithSprite.hpp"
#include "../utils/ProgressionStageUtil.hpp"
#include "../utils/StringUtil.hpp"
#include "../utils/ScrollUtil.hpp"
#include "../utils/DialogsUtil.hpp"
#include "../defines/ProgressionTable.hpp"
#include "../defines/Fonts.hpp"
#include "../defines/SquareSprites.hpp"
#include "../defines/IconSprites.hpp"

using namespace managers;

bool ProgressionTablePopup::setup(LevelProgression* value) {
    LMDEBUG("Initializing ProgessionTable");
    m_progression = value;

    setupMenus();
    setupBasics();
    setupManipulationButtons();
    setupSearchControls();
    setupProgressList();
    loadProgression();
    saveProgression();

    return true;
}

void ProgressionTablePopup::setupMenus() {
    // root menu
    m_rootMenu = CCMenu::create();
    m_rootMenu->setAnchorPoint({ 0, 1 });
    m_rootMenu->setID("progression-table-root-menu");
    m_rootMenu->setContentSize(m_mainLayer->getContentSize() - CCSize({ ROOT_MENU_MARGIN, ROOT_MENU_MARGIN }));
    m_rootMenu->setLayout(ColumnLayout::create()
                                      ->setAxisReverse(true)
                                      ->setGap(0)
                                      ->setAxisAlignment(AxisAlignment::End));

    m_mainLayer->addChildAtPosition(m_rootMenu, Anchor::TopLeft, { ROOT_MENU_MARGIN / 2, -ROOT_MENU_MARGIN / 2 });

    // top menu
    m_topMenu = CCMenu::create();
    m_topMenu->ignoreAnchorPointForPosition(false);
    m_topMenu->setID("progression-table-top-menu");
    m_topMenu->setContentSize({ m_rootMenu->getContentWidth(), TOP_MENU_HEIGHT });

    m_rootMenu->addChild(m_topMenu);

    // manipulation buttons menu
    m_manipulationButtonsMenu = CCMenu::create();
    m_manipulationButtonsMenu->ignoreAnchorPointForPosition(false);
    m_manipulationButtonsMenu->setAnchorPoint({ 1, 0.5 });
    m_manipulationButtonsMenu->setID("progression-table-manipulation-buttons");
    m_manipulationButtonsMenu->setLayout(RowLayout::create()
                                                  ->setAxisReverse(true)
                                                  ->setGap(0)
                                                  ->setAxisAlignment(AxisAlignment::End));

    m_topMenu->addChildAtPosition(m_manipulationButtonsMenu, Anchor::Right);

    // search menu
    m_searchMenu = CCMenu::create();
    m_searchMenu->ignoreAnchorPointForPosition(false);
    m_searchMenu->setID("progression-table-search-menu");
    m_searchMenu->setContentSize({ m_rootMenu->getContentWidth(), SEARCH_MENU_HEIGHT });
    m_searchMenu->setLayout(RowLayout::create()
                                        ->setAxisReverse(false)
                                        ->setAutoScale(false)
                                        ->setAxisAlignment(AxisAlignment::Center));


    m_rootMenu->addChild(m_searchMenu);

    //progression menu
    m_progressionMenu = CCMenu::create();
    m_progressionMenu->ignoreAnchorPointForPosition(false);
    m_progressionMenu->setID("progression-table-progression-menu");
    m_progressionMenu->setContentSize({ m_rootMenu->getContentWidth(), 
            m_rootMenu->getContentHeight() - m_topMenu->getContentHeight() - m_searchMenu->getContentHeight() });

    m_rootMenu->addChild(m_progressionMenu);

    // updating
    m_rootMenu->updateLayout();
}

void ProgressionTablePopup::setupBasics() {
    // title
    m_titleLabel = CCLabelBMFont::create(POPUP_TITLE, BIGFONT);
    m_titleLabel->ignoreAnchorPointForPosition(false);
    m_titleLabel->setID("progression-table-title-label");
    m_titleLabel->setScale(TITLE_SCALE);

    m_topMenu->addChildAtPosition(m_titleLabel, Anchor::Center);

    // background color
    auto background = getChildByType<CCLayer*>(0)->getChildByType<CCScale9Sprite*>(0);
    background->setColor(BACKGROUND_COLOR_CCC3B);

    // updating
    m_topMenu->updateLayout();
}

void ProgressionTablePopup::setupManipulationButtons() {
    // find startposes
    auto checkSprite = CCSprite::createWithSpriteFrameName(FIND_START_POSES_SPRITE);
    auto findStartposButtonSprite = EditorButtonWithSprite::createNode(checkSprite, EditorBaseColor::Aqua, 1.3f, MANIPULATION_BUTTONS_HEIGHT);
    auto findStartPosesButton = CCMenuItemSpriteExtra::create(findStartposButtonSprite, this, menu_selector(ProgressionTablePopup::onFindStartPoses));
    findStartPosesButton->ignoreAnchorPointForPosition(false);
    findStartPosesButton->setID("progression-table-find-startposes");

    m_manipulationButtonsMenu->addChild(findStartPosesButton);

    // reset progress
    auto resetSprite = CCSprite::createWithSpriteFrameName(RESET_SPRITE);
    auto resetProgressButtonSprite = EditorButtonWithSprite::createNode(resetSprite, EditorBaseColor::Aqua, 1.3f, MANIPULATION_BUTTONS_HEIGHT);
    auto resetProgressButton = CCMenuItemSpriteExtra::create(resetProgressButtonSprite, this, menu_selector(ProgressionTablePopup::onResetProgress));
    resetProgressButton->ignoreAnchorPointForPosition(false);
    resetProgressButton->setID("progression-table-reset-progress");

    m_manipulationButtonsMenu->addChild(resetProgressButton);

    // delete progress
    auto deleteSprite = CCSprite::createWithSpriteFrameName(DELETE_ICON_SPRITE);
    auto deleteProgressButtonSprite = EditorButtonWithSprite::createNode(deleteSprite, EditorBaseColor::Aqua, 1.1f, MANIPULATION_BUTTONS_HEIGHT);
    auto deleteButtonProgress = CCMenuItemSpriteExtra::create(deleteProgressButtonSprite, this, menu_selector(ProgressionTablePopup::onDeleteProgress));
    deleteButtonProgress->ignoreAnchorPointForPosition(false);
    deleteButtonProgress->setID("progression-table-reset-progress");

    m_manipulationButtonsMenu->addChild(deleteButtonProgress);

    // updating
    m_manipulationButtonsMenu->updateLayout();
}

void ProgressionTablePopup::setupSearchControls() {
    // percentage input
    m_percentageInput = TextInput::create(300, "Percentages (ex: 10,25,50 ...)");
    m_percentageInput->setTextAlign(TextInputAlign::Left);
    m_percentageInput->setFilter("1234567890,");
    m_percentageInput->setAnchorPoint({ 0.5, .5f });
    m_percentageInput->setScale(0.7);
    m_percentageInput->setID("progression-table-percentage-input");

    m_searchMenu->addChild(m_percentageInput);

    // apply input
    auto applySprite = CCSprite::createWithSpriteFrameName(CHECK_ICON_SPRITE);
    auto applyButtonSprite = EditorButtonWithSprite::createNode(applySprite, EditorBaseColor::Aqua, 1.3f, m_percentageInput->getScaledContentHeight() + 2);
    auto applyButton = CCMenuItemSpriteExtra::create(applyButtonSprite, this, menu_selector(ProgressionTablePopup::onApplySearch));
    applyButton->ignoreAnchorPointForPosition(false);
    applyButton->setID("progression-table-apply-button");

    m_searchMenu->addChild(applyButton);

    // update
    m_searchMenu->updateLayout();
}

void ProgressionTablePopup::setupProgressList() {
    // no progress label
    m_noProgressLabel = CCLabelBMFont::create("Insert start-poses above to start tracking your progress!", BIGFONT);
    m_noProgressLabel->setCascadeOpacityEnabled(true);
    m_noProgressLabel->setScale(0.3);
    m_noProgressLabel->setOpacity(120);
    m_noProgressLabel->setVisible(m_progression->m_stages.size() < 1);

    m_progressionMenu->addChildAtPosition(m_noProgressLabel, Anchor::Center);

    // scroll layer
    m_stagesScrollLayer = ScrollLayer::create(m_progressionMenu->getContentSize());
    m_stagesScrollLayer->setAnchorPoint({ 0.5, 0.5 });
    m_stagesScrollLayer->ignoreAnchorPointForPosition(false);
    m_stagesScrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
                                                               ->setGap(5)
                                                               ->setAxisReverse(true)
                                                               ->setAxisAlignment(AxisAlignment::End)
                                                               ->setAutoGrowAxis(m_stagesScrollLayer->getContentHeight()));

    m_progressionMenu->addChildAtPosition(m_stagesScrollLayer, Anchor::Center);
}


void ProgressionTablePopup::onFindStartPoses(CCObject*) {
    LMDEBUG("Trying to find startposes");
    if (m_progression->m_startPosPercents.size() < 1) {
        LMWARN("No cached startposes found, trying to play through!");

        static_cast<BKPlayLayer*>(BKPlayLayer::get())->playThroughoutStartposes(m_progression);
    }

    auto joined = StringUtil::joinVector(m_progression->m_startPosPercents, ",");
    m_percentageInput->setString(joined);
}

void ProgressionTablePopup::onResetProgress(CCObject*) {
    LMDEBUG("Prompting progress reset");
    DialogsUtil::showChoice("Progress Reset", "Are you sure you want to <co>reset your entire progress?</c>", "No", "Yes", [this](auto button)
    {
        if (button == DialogsUtil::ButtonResult::Right) {
            LMINFO("Resetting progress!");
            m_progression->reset();
            loadProgression();
        }
    });
}

void ProgressionTablePopup::onDeleteProgress(CCObject*) {
    LMDEBUG("Prompting table deconstruction for the current level");
    DialogsUtil::showChoice("Table Deconstruction", "Are you sure you want to <co>remove progression table</c> for <cb>this level</c>?", "No", "Yes", [this](auto button) {
        if (button == DialogsUtil::ButtonResult::Left) {
            return;
        }

        LMDEBUG("Proceeding to deconstruct the table for the current level");
        auto removeResult = LevelProgressionManager::get()->removeProgression(PlayLayer::get()->m_level);

        if (removeResult.isOk()) {
            LMINFO("Table deconstructed successfully");
            onClose(this);
        } else {
            LMERROR("Couldn't remove table, error: {}", removeResult.unwrapErr());
            DialogsUtil::showAlert("Remove Failed", "<cr>Something went wrong</c>.. Try again");
        }
    });
}

void ProgressionTablePopup::onApplySearch(CCObject*) {
    LMDEBUG("Prompting search-apply");

    auto text = m_percentageInput->getString();
    if (text.size() < 1 || hasInvalidCharacters(text)) {
        return;
    }

    if (m_progression->m_stages.size() > 0) {
        LMWARN("Existing stages found.. Prompting to replace and reset them");

        DialogsUtil::showChoice("Replace Stages", "Are you sure you want to <co>clear and replace your stages</c>?", "No", "Yes", [text, this](auto button) {
            if (button == DialogsUtil::ButtonResult::Left) {
                return;
            }

            LMINFO("Replacing stages for the current level");
            parseProgression(text);
            loadProgression();
        });
    }
    else {
        LMDEBUG("Applying search without replace");
        parseProgression(text);
        loadProgression();
    }
}

void ProgressionTablePopup::onStageCheck(StageNode* node, bool checked) {
    LMDEBUG("Handling stage check");
    if (checked) {
        handleChecked(node);
    } else {
        handleUnchecked(node);
    }
}

void ProgressionTablePopup::onClose(CCObject* x) {
    LMDEBUG("Table is closing...");
    m_scrollLayerPositionY = m_stagesScrollLayer->m_contentLayer->getPositionY();
    saveProgression();
    Popup::onClose(x);
}


void ProgressionTablePopup::handleChecked(StageNode* node) {
    LMDEBUG("Handling stage on-check");

    uint32_t index = node->getStageIndex();
    bool lastStage = (index + 1) == m_stageNodes.size();

    // current stage
    node->setPassed(true, false);
    node->setActive(false);

    // next stage
    if (!lastStage) {
        auto nextStage = m_stageNodes.at(index + 1);
        nextStage->setActive(true);
        nextStage->setEnabled(true);
    }
}

void ProgressionTablePopup::handleUnchecked(StageNode* node) {
    LMDEBUG("Handling stage off-check");

    uint32_t index = node->getStageIndex();
    bool lastStage = (index + 1) == m_stageNodes.size();

    // current stage
    node->setPassed(false, false);
    node->setActive(true);

    if (lastStage) {
        return;
    }

    // all next stages
    for (uint32_t i = 0; i < m_stageNodes.size(); i++) {
        if (i <= index) {
            continue;
        }

        auto nextStage = m_stageNodes.at(i);
        nextStage->setActive(false);
        nextStage->setPassed(false, true, false);
        nextStage->setEnabled(false);
    }
}


void ProgressionTablePopup::saveProgression() {
    LMDEBUG("Trying to save progress");
    auto result = LevelProgressionManager::get()->saveProgression(PlayLayer::get()->m_level, m_progression);

    if (!result.isOk()) {
        LMERROR("Couldn't save level progression:((");
    } else {
        LMINFO("Save progression successfully");
    }
}

void ProgressionTablePopup::parseProgression(const std::string text) {
    LMDEBUG("Proceeding to parse progression");

    std::set<uint32_t> parsed;

    if (!text.find(","))
    {
        int val = geode::utils::numFromString<uint32_t>(text).unwrapOr(0);
        if (!(val < 1 || val > 99)) {
            parsed.insert(val);
        }
    }
    else
    {
        auto split = geode::utils::string::split(text, ",");
        std::transform(split.begin(), split.end(), std::inserter(parsed, parsed.end()), [](const std::string& str)
        {
            return geode::utils::numFromString<uint32_t>(str).unwrapOr(0);
        });
    }

    auto stages = ProgressionStageUtil::createStages(&parsed);
    m_progression->m_stages = stages;
    m_progression->m_stages[0].m_isActive = true;

    m_scrollLayerPositionY = std::nullopt; // discarding
}

void ProgressionTablePopup::loadProgression() {
    LMDEBUG("Proceeding to load progression");

    m_noProgressLabel->setVisible(m_progression->m_stages.size() < 1);
    m_stagesScrollLayer->m_contentLayer->removeAllChildren();
    m_stageNodes = {};

    for (auto& stage : m_progression->m_stages)
    {
        auto node = StageNode::createNode(&stage, m_stagesScrollLayer->m_contentLayer->getContentWidth());
        node->onCheck([&](StageNode* node, bool checked) { ProgressionTablePopup::onStageCheck(node, checked); });
        
        m_stagesScrollLayer->m_contentLayer->addChild(node);
        m_stageNodes.push_back(node);
    }
    
    m_stagesScrollLayer->m_contentLayer->updateLayout();

    if (m_scrollLayerPositionY == std::nullopt) {
        ScrollUtil::scrollToTop(m_stagesScrollLayer);
    } else {
        ScrollUtil::scrollToPosition(m_stagesScrollLayer, m_scrollLayerPositionY.value());
    }
}


bool ProgressionTablePopup::hasInvalidCharacters(const std::string str) {
    std::regex validPattern("^[0-9,]*$");
    return !std::regex_match(str, validPattern);
}

ProgressionTablePopup* ProgressionTablePopup::create(LevelProgression* progression) {
    auto popup = new ProgressionTablePopup();
    if (popup->initAnchored(POPUP_SIZE_W, POPUP_SIZE_H, progression, WHITE_SQUARE_SPRITE)) {
        popup->autorelease();
        return popup;
    }

    CC_SAFE_DELETE(popup);
    return nullptr;
}