#include "ProgressNode.hpp"
#include "../../managers/SettingsManager.hpp"
#include "../../utils/TypeConversionUtil.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/Fonts.hpp"

#define EMPTY_STRING ""

bool ProgressNode::init(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses) {
    LMDEBUG("Initializing ProgressNode");

    if (!CCNode::init()) {
        LMERROR("Failed to initialize ProgressNode (CCNode init failed)");
        return false;
    }

    m_progressInfo = progressInfo;
    setContentSize({ width, PROGRESS_HEIGHT });

    setupMenu();
    setupDescriptions(descriptorsPoses);
    updateDescriptionValues();

    registerCheckable(m_passToggler);
    return true;
}

void ProgressNode::setupMenu() {
    // menu
    m_menu = CCMenu::create(); // not using layouts cuz they are incostintent in terms of aligning with descriptors
    m_menu->setContentSize(getContentSize() - CCSize({ 10, 0 }));
    m_menu->ignoreAnchorPointForPosition(false);
    m_menu->setAnchorPoint({ 0.5, 0.5 });
    m_menu->setID("progress-menu");
    this->addChildAtPosition(m_menu, Anchor::Center);
}

void ProgressNode::setupDescriptions(const std::vector<float>* descriptorsPoses) {
    // flags
    auto flags = managers::SettingsManager::get()->getProgressionSettings();
    uint32_t currentPos = 0;

    // progress (from-to)
    m_menu->addChildAtPosition(createProgressLabel(), Anchor::Left, { descriptorsPoses->at(currentPos++), 0 });

    // show best run
    if ((flags & PSFlags::ShowBestRun) != PSFlags::None) {
        m_menu->addChildAtPosition(createBestRunLabel(), Anchor::Left, { descriptorsPoses->at(currentPos++), 0 });
    }

    // show pass amount
    if ((flags & PSFlags::ShowPassAmount) != PSFlags::None) {
        m_menu->addChildAtPosition(createPassAmountLabel(), Anchor::Left, { descriptorsPoses->at(currentPos++), 0 });
    }

    // show attempts
    if ((flags & PSFlags::ShowAttempts) != PSFlags::None) {
        m_menu->addChildAtPosition(createAttemptsLabel(), Anchor::Left, { descriptorsPoses->at(currentPos++), 0 });
    }

    // toggler
    m_menu->addChildAtPosition(createPassToggler(), Anchor::Left, { descriptorsPoses->at(currentPos), 0 });
}

void ProgressNode::updateDescriptionValues() {
    LMDEBUG("Updating the values for the ProgressNode");

    // progress label
    auto progressString = std::format("{0}% - {1}%",
                                m_progressInfo->m_fromPercent,
                                m_progressInfo->m_toPercent);

    m_progressLabel->setString(progressString.c_str());
    m_progressLabel->setScale(PROGRESS_ITEM_MAX_WIDTH / m_progressLabel->getContentWidth());

    // best run label
    if (m_bestRunLabel != nullptr) {
        auto bestRunString = std::format("{0}% - {1}%",
                              TypeConversionUtil::doubleToString(m_progressInfo->m_bestRun.m_fromPercent, 1),
                              TypeConversionUtil::doubleToString(m_progressInfo->m_bestRun.m_toPercent, 1));
    
        m_bestRunLabel->setString(bestRunString.c_str());
        m_bestRunLabel->setScale(BEST_RUN_ITEM_MAX_WIDTH / m_bestRunLabel->getContentWidth());
    }

    // pass amount label
    if (m_passAmountLabel != nullptr) {
        auto passAmountString = std::format("{0} times", m_progressInfo->m_passAmount);
        m_passAmountLabel->setString(passAmountString.c_str());
        m_passAmountLabel->setScale(PASS_AMOUNT_MAX_WIDTH / m_passAmountLabel->getContentWidth());
    }

    // attempts label
    if (m_attemptsLabel != nullptr) {
        auto attemptsString = std::format("{0} att.", m_progressInfo->m_attemptsToPass);
        m_attemptsLabel->setString(attemptsString.c_str());
        m_attemptsLabel->setScale(ATTEMPTS_MAX_WIDTH / m_attemptsLabel->getContentWidth());
    }

    // pass toggle
    m_passToggler->toggle(m_progressInfo->m_isPassed);
}

CCLabelBMFont* ProgressNode::createProgressLabel() {
    m_progressLabel = CCLabelBMFont::create(EMPTY_STRING, BIGFONT);
    m_progressLabel->setAnchorPoint({ 0.5, 0.5 });
    m_progressLabel->ignoreAnchorPointForPosition(false);
    m_progressLabel->setID("progress-from-to");
    return m_progressLabel;
}

CCLabelBMFont* ProgressNode::createBestRunLabel() {
    m_bestRunLabel = CCLabelBMFont::create(EMPTY_STRING, BIGFONT);
    m_bestRunLabel->setAnchorPoint({ 0.5, 0.5 });
    m_bestRunLabel->ignoreAnchorPointForPosition(false);
    m_bestRunLabel->setID("progress-best-run");
    return m_bestRunLabel;
}

CCLabelBMFont* ProgressNode::createPassAmountLabel() {
    m_passAmountLabel = CCLabelBMFont::create(EMPTY_STRING, BIGFONT);
    m_passAmountLabel->setAnchorPoint({ 0.5, 0.5 });
    m_passAmountLabel->ignoreAnchorPointForPosition(false);
    m_passAmountLabel->setID("progress-pass-amount");
    return m_passAmountLabel;
}

CCLabelBMFont* ProgressNode::createAttemptsLabel() {
    m_attemptsLabel = CCLabelBMFont::create(EMPTY_STRING, BIGFONT);
    m_attemptsLabel->setAnchorPoint({ 0.5, 0.5 });
    m_attemptsLabel->ignoreAnchorPointForPosition(false);
    m_attemptsLabel->setID("progress-attempts");
    return m_attemptsLabel;
}

CCMenuItemToggler* ProgressNode::createPassToggler() {
    m_passToggler = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 1.f);
    m_passToggler->setScale(0.4);
    m_passToggler->setAnchorPoint({ 0.5, 0.5 });
    m_passToggler->ignoreAnchorPointForPosition(false);
    m_passToggler->setID("progress-pass-toggler");
    return m_passToggler;
}


void ProgressNode::onCheckImpl(CCObject* sender) {
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    if (!toggler) {
        return;
    }

    m_progressInfo->m_isPassed = !(toggler->m_toggled); // reversing intentionally
    m_checkFunc(this, !toggler->m_toggled);
}


void ProgressNode::setEnabled(bool isTrue) {
    LMDEBUG("Changing state 'enabled' for ProgressNode");
    m_menu->setEnabled(isTrue);
}

void ProgressNode::setPassed(bool isTrue) {
    LMDEBUG("Changing state 'passed' for ProgressNode");
    m_progressInfo->m_isPassed = isTrue;
    m_passToggler->toggle(isTrue);
}


bool ProgressNode::isChecked() {
    return m_passToggler->m_toggled;
}