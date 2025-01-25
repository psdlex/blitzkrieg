#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include "LevelProgressionManager.hpp"
#include "../utils/PathUtil.hpp"

Result<> LevelProgressionManager::init()
{
    auto levelFolder = PathUtil::get()->getLevelsFolder();
    auto created = file::createDirectory(*levelFolder);
    return created;
}

Result<LevelProgression*> LevelProgressionManager::getCachedProgression(GJGameLevel* const level)
{
    auto levelKeyResult = getLevelKey(level);

    if (!levelKeyResult.isOk()) {
        return Err(levelKeyResult.unwrapErr());
    }

    auto levelKey = levelKeyResult.unwrap();

    if (m_cachedLevelProgressions.contains(levelKey)) {
        return Ok(&m_cachedLevelProgressions[levelKey]);
    }

    return Err("Object not found");
}

Result<LevelProgression*> LevelProgressionManager::getProgression(GJGameLevel* const level)
{
    auto levelKeyResult = getLevelKey(level);

    if (!levelKeyResult.isOk()) {
        return Err(levelKeyResult.unwrapErr());
    }

    auto levelKey = levelKeyResult.unwrap();
    auto fullPath = *PathUtil::get()->getLevelsFolder() / (levelKey + ".json");
    auto levelProgression = file::readFromJson<LevelProgression>(fullPath);

    if (!levelProgression.isOk()) {
        return Err("Couldnt read from json.");
    }

    m_cachedLevelProgressions[levelKey] = levelProgression.unwrap();
    return Ok(&m_cachedLevelProgressions[levelKey]);
}

Result<LevelProgression*> LevelProgressionManager::createEmptyProgression(GJGameLevel* const level)
{
    auto progression = LevelProgression();
    auto levelKeyResult = getLevelKey(level);

    if (!levelKeyResult.isOk()) {
        return Err(levelKeyResult.unwrapErr());
    }

    auto levelKey = levelKeyResult.unwrap();
    bool result = setProgression(level, &progression, &levelKey);

    if (!result) {
        return Err("Something went wrong");
    }

    m_cachedLevelProgressions[levelKey] = progression;
    return Ok(&m_cachedLevelProgressions[levelKey]);
}

bool LevelProgressionManager::setProgression(const GJGameLevel* level, const LevelProgression* levelProgression, const gd::string* levelKey)
{
    auto fullPath = *PathUtil::get()->getLevelsFolder() / (*levelKey + ".json");
    auto result = file::writeToJson(fullPath, *levelProgression);

    return result.isOk();
}


// copy-pasted from: https://github.com/eloh-mrow/death-tracker
Result<std::string> LevelProgressionManager::getLevelKey(GJGameLevel* const level) {
    if (!level) {
        return Err("invalid level!");
    }

    std::string levelId;

    if (level->m_levelType == GJLevelType::Editor) {
        levelId += std::to_string(EditorIDs::getID(level));
    }
    else {
        levelId += std::to_string(level->m_levelID.value());
    }

    // local level postfix
    if (level->m_levelType == GJLevelType::Local) {
        levelId += "-local";
    }

    if (level->m_levelType == GJLevelType::Editor) {
        levelId += "-editor";
    }

    // daily/weekly postfix
    if (level->m_dailyID > 0) {
        levelId += "-daily";
    }

    // gauntlet level postfix
    if (level->m_gauntletLevel) {
        levelId += "-gauntlet";
    }

    return Ok(levelId);
}