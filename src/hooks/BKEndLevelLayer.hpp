#pragma once
#ifndef BK_END_LEVEL_LAYER_HPP
#define BK_END_LEVEL_LAYER_HPP

#include "Geode/Geode.hpp"
#include "Geode/modify/EndLevelLayer.hpp"
#include "../objects/LevelProgression.hpp"

using namespace geode::prelude;

class $modify(BKEndLevelLayer, EndLevelLayer) {

    struct Fields {
        LevelProgression* m_levelProgression;
    };

    static void onModify(auto& self) {
        (void)self.setHookPriorityPost("EndLevelLayer::customSetup", Priority::Last);
    }

    void customSetup();
    void onShowProgressionTable(CCObject*);
    void showProgressionTable(LevelProgression* levelProgression);
};

#endif // BK_END_LEVEL_LAYER_HPP