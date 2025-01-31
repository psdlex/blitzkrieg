#include "BKPlayLayer.hpp"
#include "../managers/LogManager.hpp"

bool BKPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    LMDEBUG("Initializing BKPlayLayer");
    auto settings = Mod::get()->getSavedSettingsData();

    LMDEBUG("Obtaining cached progression for current level");    
    auto cached = LevelProgressionManager::get()->getCachedProgression(level);
    if (cached.isOk()) {
        LMINFO("Cached progression found");    
        m_fields->m_progression = cached.unwrap();
    }
    else {
        LMINFO("Cached progression not found, trying to read the file");    
        auto fileProgression = LevelProgressionManager::get()->getProgression(level);
        if (fileProgression.isOk()) {
            LMWARN("Filed progression not found");    
            m_fields->m_progression = fileProgression.unwrap();
        }
    }
    
    return PlayLayer::init(level, useReplay, dontCreateObjects);
}

void BKPlayLayer::addObject(GameObject* obj) {
    PlayLayer::addObject(obj);
    if (obj->m_objectID == 31) { // startpos ID
        m_fields->m_startPoses.push_back(static_cast<StartPosObject*>(obj));
    }
}

void BKPlayLayer::setupHasCompleted() {
    auto startposeAmount = m_fields->m_startPoses.size();
    LMINFO("Startposes found in the current level: {}", startposeAmount);

    PlayLayer::setupHasCompleted();
    playThroughoutStartposes(m_fields->m_progression);
}

void BKPlayLayer::playThroughoutStartposes(LevelProgression* progression) {
    if (progression == nullptr || m_fields->m_startPoses.size() < 1) {
        return;
    }

    LMDEBUG("Playing through startposes to obtain their percentage using game's measurements");

    progression->m_startPosPercents.clear();
    for (auto& startpos : m_fields->m_startPoses) {
        setStartPosObject(startpos);
        resetLevel();
        progression->m_startPosPercents.push_back(getCurrentPercentInt());
    }
}