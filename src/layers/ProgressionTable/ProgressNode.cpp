#include "../ProgressionTable/ProgressNode.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/Fonts.hpp"
#include "../../utils/TypeConversionUtil.hpp"
#include "../../managers/SettingsManager.hpp"

bool ProgressNode::init(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses)
{
    if (!CCNode::init())
        return false;

    m_progressInfo = progressInfo;
    setContentSize({ width, PROGRESS_HEIGHT });

    // flags
    auto flags = managers::SettingsManager::get()->getProgressionSettings();
    uint32_t currentPos = 0;

    // menu
    m_menu = CCMenu::create(); // not using layouts cuz they are way too incostintent in terms of aligning with descriptors, sorry good code :(
    m_menu->setContentSize(getContentSize() - CCSize({ 10, 0 }));
    m_menu->ignoreAnchorPointForPosition(false);
    m_menu->setAnchorPoint({ 0.5, 0.5 });
    m_menu->setID("progress-menu");

    this->addChildAtPosition(m_menu, Anchor::Center);

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

    registerCheckable(m_passToggler);
    return true;
}

CCLabelBMFont* ProgressNode::createProgressLabel()
{
    auto progressString = std::format("{0}% - {1}%",
                    m_progressInfo->m_fromPercent,
                    m_progressInfo->m_toPercent);
    
    auto progressLabel = CCLabelBMFont::create(progressString.c_str(), BIGFONT);
    progressLabel->setAnchorPoint({ 0.5, 0.5 });
    progressLabel->ignoreAnchorPointForPosition(false);
    progressLabel->setID("progress-from-to");
    progressLabel->setScale(PROGRESS_ITEM_MAX_WIDTH / progressLabel->getContentWidth());
    return progressLabel;
}

CCLabelBMFont* ProgressNode::createBestRunLabel()
{
    auto bestRunString = std::format("{0}% - {1}%",
                TypeConversionUtil::doubleToString(m_progressInfo->m_bestRun.m_fromPercent, 1),
                TypeConversionUtil::doubleToString(m_progressInfo->m_bestRun.m_toPercent, 1));
    
    auto bestRunLabel = CCLabelBMFont::create(bestRunString.c_str(), BIGFONT);
    bestRunLabel->setAnchorPoint({ 0.5, 0.5 });
    bestRunLabel->ignoreAnchorPointForPosition(false);
    bestRunLabel->setID("progress-best-run");
    bestRunLabel->setScale(BEST_RUN_ITEM_MAX_WIDTH / bestRunLabel->getContentWidth());
    return bestRunLabel;
}

CCLabelBMFont* ProgressNode::createPassAmountLabel()
{
    auto passAmountString = std::format("{0} times", m_progressInfo->m_passAmount);
    auto passAmountLabel = CCLabelBMFont::create(passAmountString.c_str(), BIGFONT);
    passAmountLabel->setAnchorPoint({ 0.5, 0.5 });
    passAmountLabel->ignoreAnchorPointForPosition(false);
    passAmountLabel->setID("progress-pass-amount");
    passAmountLabel->setScale(PASS_AMOUNT_MAX_WIDTH / passAmountLabel->getContentWidth());
    return passAmountLabel;
}

CCLabelBMFont* ProgressNode::createAttemptsLabel()
{
    auto attemptsString = std::format("{0} att.", m_progressInfo->m_attemptsToPass);
    auto attemptsLabel = CCLabelBMFont::create(attemptsString.c_str(), BIGFONT);
    attemptsLabel->setAnchorPoint({ 0.5, 0.5 });
    attemptsLabel->ignoreAnchorPointForPosition(false);
    attemptsLabel->setID("progress-attempts");
    attemptsLabel->setScale(ATTEMPTS_MAX_WIDTH / attemptsLabel->getContentWidth());
    return attemptsLabel;
}

CCMenuItemToggler* ProgressNode::createPassToggler()
{
    m_passToggler = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 1.f);
    m_passToggler->setScale(0.4);
    m_passToggler->setAnchorPoint({ 0.5, 0.5 });
    m_passToggler->ignoreAnchorPointForPosition(false);
    m_passToggler->setID("progress-pass-toggler");
    m_passToggler->toggle(m_progressInfo->m_isPassed);

    return m_passToggler;
}

void ProgressNode::onCheckImpl(CCObject* sender)
{
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    if (!toggler) {
        return;
    }

    m_progressInfo->m_isPassed = !toggler->m_toggled; // reversing intentionally
    m_checkFunc(this, !toggler->m_toggled);
}

ProgressNode* ProgressNode::create(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses)
{
    auto ret = new ProgressNode();
    if (ret->init(progressInfo, width, descriptorsPoses)) {
        ret->autorelease();  
        return ret;
    }

    delete ret;
    return nullptr;
}

void ProgressNode::setEnabled(bool isTrue)
{
    m_menu->setEnabled(isTrue);
}

void ProgressNode::setPassed(bool isTrue)
{
    m_progressInfo->m_isPassed = isTrue;
    m_passToggler->toggle(isTrue);
}

bool ProgressNode::isChecked()
{
    return m_passToggler->m_toggled;
}