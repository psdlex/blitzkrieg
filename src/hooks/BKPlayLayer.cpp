#include "../hooks/BKPlayLayer.hpp"

bool BKPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    auto settings = Mod::get()->getSavedSettingsData();

    auto cached = LevelProgressionManager::get()->getCachedProgression(level);
    if (cached.isOk()) {
        m_fields->m_progression = cached.unwrap();
    }
    else
    {
        auto fileProgression = LevelProgressionManager::get()->getProgression(level);
        if (fileProgression.isOk()) {
            m_fields->m_progression = fileProgression.unwrap();
        }
    }
    
    return PlayLayer::init(level, useReplay, dontCreateObjects);
}

void BKPlayLayer::addObject(GameObject* obj)
{
    PlayLayer::addObject(obj);

    if (obj->m_objectID == 31) { // startpos ID
        m_fields->m_startPoses.push_back(static_cast<StartPosObject*>(obj));
    }
}

void BKPlayLayer::setupHasCompleted()
{
    PlayLayer::setupHasCompleted();
    playThroughoutStartposes(m_fields->m_progression);
}

void BKPlayLayer::playThroughoutStartposes(LevelProgression* progression)
{
    if (progression == nullptr || m_fields->m_startPoses.size() < 1) {
        return;
    }

    progression->m_startPosPercents.clear();
    for (auto& startpos : m_fields->m_startPoses)
    {
        setStartPosObject(startpos);
        resetLevel();
        progression->m_startPosPercents.push_back(getCurrentPercentInt());
    }
}