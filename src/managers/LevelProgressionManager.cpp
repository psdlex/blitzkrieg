#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include "LevelProgressionManager.hpp"
#include "PathManager.hpp"

using namespace managers;

Result<> LevelProgressionManager::init()
{
    auto levelsFolder = PathManager::get()->getLevelsFolder();
    auto created = file::createDirectory(*levelsFolder);
    return !created.isOk() ? Err("LevelProgressionManager: failed to create a folder") : created;
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

    return Err("Progression cache not found");
}

Result<LevelProgression*> LevelProgressionManager::getProgression(GJGameLevel* const level)
{
    auto levelKeyResult = getLevelKey(level);

    if (!levelKeyResult.isOk()) {
        return Err(levelKeyResult.unwrapErr());
    }

    auto levelKey = levelKeyResult.unwrap();
    auto fullPath = getLevelPath(level, &levelKey);

    if (!std::filesystem::exists(fullPath)) {
        return Err("Progression for this level doesnt exist");
    }

    auto levelProgression = file::readFromJson<LevelProgression>(fullPath);

    if (!levelProgression.isOk()) {
        return Err("Couldn't read level progression from json");
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
    auto result = setProgression(level, &progression);

    if (!result.isOk()) {
        return Err(result.unwrapErr());
    }

    m_cachedLevelProgressions[levelKey] = progression;
    return Ok(&m_cachedLevelProgressions[levelKey]);
}

Result<> LevelProgressionManager::setProgression(GJGameLevel* const level, const LevelProgression* const levelProgression)
{
    auto levelKeyResult = getLevelKey(level);
    if (!levelKeyResult.isOk()) {
        return Err(levelKeyResult.unwrapErr());
    }

    auto levelKey = levelKeyResult.unwrap();
    auto fullPath = getLevelPath(level, &levelKey);
    auto result = file::writeToJson(fullPath, *levelProgression);
    return !result.isOk() ? Err("Couldnt save level progress into json") : result; 
}

Result<> LevelProgressionManager::removeProgression(GJGameLevel* const level)
{
    auto levelKeyResult = getLevelKey(level);
    if (!levelKeyResult.isOk()) {
        return Err(levelKeyResult.unwrapErr());
    }

    auto levelKey = levelKeyResult.unwrap();
    auto fullPath = getLevelPath(level, &levelKey);
    auto result = std::filesystem::remove(fullPath);
    
    if (!result) {
        return Err("Couldn't remove level progression file");
    }

    m_cachedLevelProgressions.erase(levelKey);
    return Ok();
}

std::filesystem::path LevelProgressionManager::getLevelPath(GJGameLevel* const level, const gd::string* levelKey)
{
    return *PathManager::get()->getLevelsFolder() / (*levelKey + ".json");
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