#include "../ProgressionTable/ProgressNode.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/Fonts.hpp"
#include "../../utils/TypeConversionUtil.hpp"
#include "../../utils/SettingsUtil.hpp"

typedef SettingsUtils::ProgressionSettingsFlags PSFlags;

bool ProgressNode::init(ProgressInfo* progressInfo, float width, const std::vector<float>* descriptorsPoses)
{
    if (!CCNode::init())
        return false;

    m_progressInfo = progressInfo;
    setContentSize({ width, PROGRESS_HEIGHT });

    // flags
    auto flags = SettingsUtils::getProgressionSettings();
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
    m_menu->addChildAtPosition(createPassToggler(), Anchor::Left, { descriptorsPoses->at(currentPos++), 0 });

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
                TypeConversionUtil::doubleToString(m_progressInfo->m_bestRun.m_fromPercent, 2),
                TypeConversionUtil::doubleToString(m_progressInfo->m_bestRun.m_toPercent, 2));
    
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
    auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ProgressNode::onProgressCheck), 1.f);
    toggler->setScale(0.4);
    toggler->setAnchorPoint({ 0.5, 0.5 });
    toggler->ignoreAnchorPointForPosition(false);
    toggler->setID("progress-pass-toggler");
    return toggler;
}

void ProgressNode::onProgressCheck(CCObject* object)
{

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