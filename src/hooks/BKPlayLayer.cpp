#include "BKPlayLayer.hpp"
#include "../managers/LogManager.hpp"
#include "../defines/PlayLayer.hpp"
#include "chrono"
#include "thread"

bool BKPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    LMDEBUG("Initializing BKPlayLayer");
    
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
}

void BKPlayLayer::playThroughoutStartposes(LevelProgression* progression) {
    if (progression == nullptr || m_fields->m_startPoses.size() < 1) {
        return;
    }

    LMDEBUG("Playing through startposes to obtain their percentage using game's measurements");

    progression->m_startPosPercents.clear();
    int32_t lastPercent = -1.0f;

    for (auto& startpos : m_fields->m_startPoses) {
        bool timedOut = true;
        setStartPosObject(startpos);
        resetLevel();
        
        // basically timeout unless startpos switches on difficult to render levels
        for (uint32_t i = 0; i < STARTPOS_SWITCH_TIMEOUT_MS; i += STARTPOS_SWITCH_SLEEP_DURATION_MS) {
            if (lastPercent != getCurrentPercentInt()) {
                timedOut = false;
                lastPercent = getCurrentPercentInt();
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(STARTPOS_SWITCH_SLEEP_DURATION_MS));
        }

        if (timedOut) {
            LMERROR("Timed out while seeking startposes.. (buy a new computer)");
            break;
        }

        progression->m_startPosPercents.push_back(lastPercent);
    }
}