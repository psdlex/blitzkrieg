#pragma once
#ifndef PATH_UTIL_HPP
#define PATH_UTIL_HPP

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PathUtil
{
	static std::filesystem::path levelsFolder;

public:
	static Result<> init() 
	{
		try {
			levelsFolder = Mod::get()->getSaveDir() / "levels";
		} catch (auto& ex) {
			return Err("Something went wrong while obtaining save dir");
		}

		return Ok();
	} 

	static std::filesystem::path* getLevelsFolder() { return &levelsFolder; }

private:
	PathUtil() {}
};

inline std::filesystem::path PathUtil::levelsFolder;

#endif // PATH_UTIL_HPP