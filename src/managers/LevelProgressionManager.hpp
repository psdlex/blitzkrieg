#pragma once
#ifndef LEVEL_PROGRESSION_MANAGER_HPP
#define LEVEL_PROGRESSION_MANAGER_HPP

#include <Geode/Geode.hpp>
#include "../serializers/GenericSerializers.hpp"
#include "../abstraction/SingletonBase.hpp"
#include "abstraction/Initializable.hpp"

using namespace geode::prelude;

namespace managers
{
	class LevelProgressionManager : public SingletonBase<LevelProgressionManager>, public Initializable
	{
		friend class SingletonBase<LevelProgressionManager>;

	private:
		std::map<std::string, LevelProgression> m_cachedLevelProgressions;

	public:
		Result<> LevelProgressionManager::init() override;

		Result<LevelProgression*> getCachedProgression(GJGameLevel* const level);
		Result<LevelProgression*> getProgression(GJGameLevel* const level);
		Result<LevelProgression*> createEmptyProgression(GJGameLevel* const level);

		Result<> setProgression(GJGameLevel* const level, const LevelProgression* const levelProgression);
		Result<> removeProgression(GJGameLevel* const level);

	private:
		LevelProgressionManager() {};
		Result<std::string> getLevelKey(GJGameLevel* const level);
		std::filesystem::path getLevelPath(GJGameLevel* const level, const gd::string* levelKey);
	};
};

#endif // LEVEL_PROGRESSION_MANAGER_HPP