#include "BKPlayLayerTracker.hpp"
#include "../managers/LevelProgressionManager.hpp"
#include "../managers/LogManager.hpp"
#include "../managers/SettingsManager.hpp"

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

void BKPlayLayerTracker::destroyPlayer(PlayerObject* player, GameObject* p1) {
    PlayLayer::destroyPlayer(player, p1);

    if (!player->m_isDead) {
        return;
    }

    auto deathPercent = getCurrentPercent();
    processProgress(deathPercent);
}

void BKPlayLayerTracker::levelComplete() {
    PlayLayer::levelComplete();
    processProgress(100.0f);
}

void BKPlayLayerTracker::processProgress(float toPercent) {
    auto currentProgress = m_fields->m_currentProgress;

    if (currentProgress == nullptr) {
        return;
    }

    // increasing pass amount
    if (currentProgress->m_toPercent <= static_cast<int>(toPercent)) {
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

    // checking  stage if all progresses are passed
    if (m_fields->m_currentStage->allProgressesPassed()) {
        m_fields->m_currentStage->m_isPassed = true;
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