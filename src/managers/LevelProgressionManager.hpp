#pragma once
#ifndef LEVEL_PROGRESSION_MANAGER_HPP
#define LEVEL_PROGRESSION_MANAGER_HPP

#include <Geode/Geode.hpp>
#include "../serializers/GenericSerializers.hpp"
#include "../utils/PathUtil.hpp"
#include "../abstraction/SingletonBase.hpp"

using namespace geode::prelude;

class LevelProgressionManager : public SingletonBase<LevelProgressionManager>
{
private:
	friend class SingletonBase<LevelProgressionManager>;
	std::map<std::string, LevelProgression> m_cachedLevelProgressions;

public:
	static Result<> init();

	Result<LevelProgression*> getCachedProgression(GJGameLevel* const level);
	Result<LevelProgression*> getProgression(GJGameLevel* const level);
	Result<LevelProgression*> createEmptyProgression(GJGameLevel* const level);
	bool setProgression(const GJGameLevel* level, const LevelProgression* levelProgression, const gd::string* levelKey);

private:
	LevelProgressionManager() {};
	geode::Result<std::string> getLevelKey(GJGameLevel* const level);
};

#endif // LEVEL_PROGRESSION_MANAGER_HPP