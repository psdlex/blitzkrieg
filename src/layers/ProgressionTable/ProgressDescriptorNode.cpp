#include "ProgressDescriptorNode.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/Fonts.hpp"

bool ProgressDescriptorNode::init(const matjson::Value* settings, float width)
{
    if (!CCNode::init()) {
        return false;
    }

    setContentSize({ width, PROGRESS_HEIGHT });

    // root menu
    m_rootMenu = CCMenu::create();
    m_rootMenu->ignoreAnchorPointForPosition(false);
    m_rootMenu->setContentSize(getContentSize());
    m_rootMenu->setAnchorPoint({ 0.5, 0.5 });
    m_rootMenu->setLayout(RowLayout::create()
                           ->setAxisAlignment(AxisAlignment::Between)
                           ->setAutoScale(false)
                           ->setAxisReverse(true));

    this->addChildAtPosition(m_rootMenu, Anchor::Center);

    // background
    auto background = CCLayerColor::create(PROGRESS_DESCRIPTOR_COLOR_CCC4);
    background->ignoreAnchorPointForPosition(false);
    background->setAnchorPoint({ 0.5, 0.5 });
    background->setContentSize(getContentSize());

    this->addChildAtPosition(background, Anchor::Center);

    // descriptors (name/scale)
    auto descriptors = createVector(SettingsUtils::get()->getProgressionSettings(settings));

    for (auto& descriptor : descriptors)
    {
        auto text = CCLabelBMFont::create(descriptor.first.c_str(), BIGFONT);
        text->ignoreAnchorPointForPosition(false);
        text->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        text->setAnchorPoint({ 0.5, 0.5 });
        text->setScale(descriptor.second);

        m_rootMenu->addChild(text);
    }

    m_rootMenu->updateLayout();

    return true;
}

std::vector<std::pair<std::string, float>> ProgressDescriptorNode::createVector(PSFlags flags)
{
    std::vector<std::pair<std::string, float>> vec = {};

    vec.push_back({ "Progress", 0.3 });
    vec.push_back({ "Passed", 0.3 });

    if (flags & (PSFlags::ShowPassAmount)) {
        vec.push_back({ "Pass\nAmount", 0.25 });
    }

    if (flags & (PSFlags::ShowBestRun)) {
        vec.push_back({ "Best Run", 0.3 });
    }

    if (flags & (PSFlags::ShowAttempts)) {
        vec.push_back({ "Attempts", 0.3 });
    }

    return vec;
}

ProgressDescriptorNode* ProgressDescriptorNode::create(const matjson::Value* settings, float width)
{
    auto ret = new ProgressDescriptorNode();
    if (ret->init(settings, width)) {
        ret->autorelease();  
        return ret;
    }

    delete ret;
    return nullptr;
}