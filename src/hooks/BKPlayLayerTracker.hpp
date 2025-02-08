#pragma once
#ifndef BK_PLAY_LAYER_TRACKER_HPP
#define BK_PLAY_LAYER_TRACKER_HPP

#include "Geode/Geode.hpp"
#include "Geode/modify/PlayLayer.hpp"
#include "../objects/LevelProgression.hpp"

class $modify(BKPlayLayerTracker, PlayLayer) {
    struct Fields {
        float m_fromPercent;
        bool m_autoCheck = false;
        LevelProgression* m_progression;
        ProgressionStage* m_currentStage;
        ProgressInfo* m_currentProgress;
    };

    void resetLevel();
    void levelComplete() ;
    void destroyPlayer(PlayerObject* p0, GameObject* p1);
    void processProgress(float toPercent);
    void saveProgression();

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
};

#endif // BK_PLAY_LAYER_TRACKER_HPP