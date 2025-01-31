#pragma once
#ifndef BK_PLAY_LAYER_HPP
#define BK_PLAY_LAYER_HPP

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../managers/LevelProgressionManager.hpp"
#include "../objects/LevelProgression.hpp"

using namespace geode::prelude;
using namespace managers;

class $modify(BKPlayLayer, PlayLayer) {
    struct Fields {
        LevelProgression* m_progression;
        std::vector<StartPosObject*> m_startPoses;
    };

    bool init(GJGameLevel* gameLevel, bool useReplay, bool dontCreateObjects);
    void addObject(GameObject* obj);
    void setupHasCompleted();

public:
    void playThroughoutStartposes(LevelProgression* progression);
};

#endif // BK_PLAY_LAYER_HPP