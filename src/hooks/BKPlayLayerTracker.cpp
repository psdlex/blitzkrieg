#include "BKPlayLayerTracker.hpp"
#include "../managers/LevelProgressionManager.hpp"
#include "../managers/LogManager.hpp"
#include "../managers/SettingsManager.hpp"
#include "../managers/SfxManager.hpp"

using namespace managers;

bool BKPlayLayerTracker::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    LMDEBUG("Initializing BKPlayLayerTracker");
    auto settings = SettingsManager::get()->getProgressionSettings();
    m_fields->m_autoCheck = (settings & PSFlags::EnableAutoChecker) != PSFlags::None;

    auto cached = LevelProgressionManager::get()->getCachedProgression(level);
    if (cached.isOk()) {
        LMINFO("Cached progression found");
        m_fields->m_progression = cached.unwrap();
    }
    else {
        LMWARN("Cached progression not found, trying to read the file");    
        auto fileProgression = LevelProgressionManager::get()->getProgression(level);
        if (fileProgression.isOk()) {
            LMWARN("Filed progression not found");    
            m_fields->m_progression = fileProgression.unwrap();
        }
    }
    
    return PlayLayer::init(level, useReplay, dontCreateObjects);
}

void BKPlayLayerTracker::resetLevel() {
    PlayLayer::resetLevel();

    LMDEBUG("Reset level called");
    m_fields->m_noclipDetected = false;
    auto currentPerc = getCurrentPercent();

    if (m_fields->m_progression == nullptr) {
        return;
    }

    auto activeStage = m_fields->m_progression->getActiveStage();
    m_fields->m_currentStage = activeStage;
    if (activeStage == nullptr) {
        LMWARN("No active stage for level: {}", m_level->m_levelName);
        return;
    }

    m_fields->m_fromPercent = currentPerc;
    m_fields->m_currentProgress = activeStage->getProgressInfoByStartingPercent(static_cast<int>(currentPerc));

    if (m_fields->m_currentProgress == nullptr) {
        LMWARN("Progress with same starting percent not found");
        return;
    }
}

void BKPlayLayerTracker::destroyPlayer(PlayerObject* player, GameObject* obj) {
    if (m_anticheatSpike && obj == m_anticheatSpike) {
        return PlayLayer::destroyPlayer(player, obj);
    }

    PlayLayer::destroyPlayer(player, obj);
    if (!player->m_isDead) {
        m_fields->m_noclipDetected = true;
        return;
    }

    if (m_fields->m_noclipDetected) {
        LMWARN("Noclip detected, progress wont be registered");
        return;
    }

    auto deathPercent = getCurrentPercent();
    processProgress(deathPercent);
}

void BKPlayLayerTracker::levelComplete() {
    PlayLayer::levelComplete();

    if (m_fields->m_noclipDetected) {
        LMWARN("Noclip detected, progress wont be registered");
        return;
    }

    processProgress(100.0f);
}

void BKPlayLayerTracker::processProgress(float toPercent) {
    auto currentProgress = m_fields->m_currentProgress;
    bool playProgressSound = false;

    if (currentProgress == nullptr) {
        return;
    }

    // increasing pass amount
    if (currentProgress->m_toPercent <= static_cast<int>(toPercent)) {
        if (currentProgress->m_isPassed == false) {
            playProgressSound = true;
        }

        currentProgress->m_passAmount++;
        if (m_fields->m_autoCheck == true) {
            currentProgress->m_isPassed = true;
        }
    }

    // setting best run
    if (currentProgress->m_bestRun.m_toPercent < toPercent) {
        currentProgress->m_bestRun.m_fromPercent = m_fields->m_fromPercent;
        currentProgress->m_bestRun.m_toPercent = toPercent;
    }
    
    // setting attempts if not set already
    if (currentProgress->m_passAmount > 0 && currentProgress->m_attemptsToPass < 1) {
        currentProgress->m_attemptsToPass = m_level->m_attempts;
    }

    // checking stage if all progresses are passed
    if (m_fields->m_autoCheck == true && m_fields->m_currentStage->allProgressesPassed()) {
        if (m_fields->m_currentStage->m_isPassed == false) {
            m_fields->m_progression->setStagePassed(m_fields->m_currentStage);
            
            playProgressSound = false;
            SFX_PLAY_STAGE_PASSED();
        }
    }

    if (playProgressSound) {
        SFX_PLAY_PROGRESS_PASSED();
    }

    saveProgression();
}

void BKPlayLayerTracker::saveProgression() {
    LMDEBUG("Trying to save progress");
    auto result = LevelProgressionManager::get()->saveProgression(PlayLayer::get()->m_level, m_fields->m_progression);

    if (!result.isOk()) {
        LMERROR("Couldn't save level progression:((");
    } else {
        LMINFO("Save progression successfully");
    }
}