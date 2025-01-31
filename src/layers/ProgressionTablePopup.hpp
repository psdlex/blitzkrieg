#pragma once
#ifndef PROGRESSION_TABLE_POPUP_HPP
#define PROGRESSION_TABLE_POPUP_HPP

#include <Geode/Geode.hpp>
#include "../objects/LevelProgression.hpp"
#include "ProgressionTable/StageNode.hpp"

using namespace geode::prelude;

class ProgressionTablePopup : public geode::Popup<LevelProgression*> {
protected:
    // data
    LevelProgression* m_progression;
    std::vector<StageNode*> m_stageNodes;
    
    // menus
    CCMenu* m_rootMenu;
    CCMenu* m_topMenu;
    CCMenu* m_manipulationButtonsMenu;
    CCMenu* m_searchMenu;
    CCMenu* m_progressionMenu;

    // other
    CCLabelBMFont* m_titleLabel;
    CCLabelBMFont* m_noProgressLabel;

    TextInput* m_percentageInput;
    ScrollLayer* m_stagesScrollLayer;

    // statics
    static std::optional<int32_t> m_scrollLayerPositionY;

protected:
    bool setup(LevelProgression* value) override;
    void onClose(CCObject* x) override;

    void parseProgression(const std::string text);
    void loadProgression();
    bool hasInvalidCharacters(const std::string str);

    void setupMenus();
    void setupBasics();
    void setupManipulationButtons();
    void setupSearchControls();
    void setupProgressList();

    void handleChecked(StageNode* node);
    void handleUnchecked(StageNode* node);
    void saveProgression();

    void onFindStartPoses(CCObject*);
    void onResetProgress(CCObject*);
    void onDeleteProgress(CCObject*);
    void onStageCheck(StageNode* node, bool checked);

    void onApplySearch(CCObject*);
    
public:
    static ProgressionTablePopup* create(LevelProgression* progression);
};

inline std::optional<int32_t> ProgressionTablePopup::m_scrollLayerPositionY = std::nullopt;

#endif // PROGRESSION_TABLE_POPUP_HPP