#pragma once
#ifndef PROGRESSION_TABLE_POPUP_HPP
#define PROGRESSION_TABLE_POPUP_HPP

#include <Geode/Geode.hpp>
#include "../objects/LevelProgression.hpp"

using namespace geode::prelude;

class ProgressionTablePopup : public geode::Popup<LevelProgression*> {
    // data
    LevelProgression* m_progression;
    
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
    Ref<ScrollLayer> m_stagesScrollLayer;

    // statics
    static int32_t m_scrollLayerPositionY;

protected:
    bool setup(LevelProgression* value) override;
    void onExit();

    void parseProgression(const std::string text);
    void loadProgression();
    bool hasInvalidCharacters(const std::string str);

    void setupMenus();
    void setupBasics();
    void setupManipulationButtons();
    void setupSearchControls();
    void setupProgressList();

    void onFindStartPoses(CCObject*);
    void onResetProgress(CCObject*);
    void onDeleteProgress(CCObject*);

    void onApplySearch(CCObject*);
public:
    static ProgressionTablePopup* create(LevelProgression* progression);
};

inline int32_t ProgressionTablePopup::m_scrollLayerPositionY = -1;

#endif // PROGRESSION_TABLE_POPUP_HPP