#include "ProgressDescriptorNode.hpp"
#include "../../defines/ProgressNode.hpp"
#include "../../defines/Fonts.hpp"

bool ProgressDescriptorNode::init(float width)
{
    if (!CCNode::init()) {
        return false;
    }

    setContentSize({ width, PROGRESS_HEIGHT });

    // root menu
    m_rootMenu = CCMenu::create();
    m_rootMenu->setID("progress-descriptor-menu");
    m_rootMenu->ignoreAnchorPointForPosition(false);
    m_rootMenu->setContentSize(getContentSize() - CCSize({ 10, 0 }));
    m_rootMenu->setAnchorPoint({ 0.5, 0.5 });
    m_rootMenu->setLayout(RowLayout::create()
                           ->setAxisAlignment(AxisAlignment::Between)
                           ->setAutoScale(false)
                           ->setAutoGrowAxis(std::nullopt)
                           ->setAxisReverse(false));

    this->addChildAtPosition(m_rootMenu, Anchor::Center);

    // background
    auto background = CCLayerColor::create(PROGRESS_DESCRIPTOR_COLOR_CCC4);
    background->setID("progress-descriptor-bg");
    background->ignoreAnchorPointForPosition(false);
    background->setAnchorPoint({ 0.5, 0.5 });
    background->setContentSize(getContentSize());

    this->addChildAtPosition(background, Anchor::Center);

    // descriptors (name/scale)
    auto descriptors = createVector(managers::SettingsManager::get()->getProgressionSettings());
    _descriptorsPositionsX = std::vector<float>();
    
    for (auto& descriptor : descriptors)
    {
        auto text = CCLabelBMFont::create(descriptor.first.c_str(), BIGFONT);
        text->ignoreAnchorPointForPosition(false);
        text->setID("progress-descriptor-item");
        text->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        text->setAnchorPoint({ 0.5, 0.5 });
        text->setScale(descriptor.second);

        m_rootMenu->addChild(text);
    }

    m_rootMenu->updateLayout();

    for (auto& child : CCArrayExt<cocos2d::CCNode*>(m_rootMenu->getChildren())) {
        _descriptorsPositionsX.push_back(child->getPositionX());
    }

    return true;
}

std::vector<std::pair<std::string, float>> ProgressDescriptorNode::createVector(PSFlags flags)
{
    std::vector<std::pair<std::string, float>> vec = {};

    vec.push_back({ "Progress", 0.3 });

    if ((flags & PSFlags::ShowBestRun) != PSFlags::None) {
        vec.push_back({ "Best Run", 0.3 });
    }

    if ((flags & PSFlags::ShowPassAmount) != PSFlags::None) {
        vec.push_back({ "Pass Amount", 0.3 });
    }

    if ((flags & PSFlags::ShowAttempts) != PSFlags::None) {
        vec.push_back({ "Attempts", 0.3 });
    }

    vec.push_back({ "Passed", 0.3 });

    return vec;
}

ProgressDescriptorNode* ProgressDescriptorNode::create(float width)
{
    auto ret = new ProgressDescriptorNode();
    if (ret->init(width)) {
        ret->autorelease();  
        return ret;
    }

    delete ret;
    return nullptr;
}