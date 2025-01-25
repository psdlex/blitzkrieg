#pragma once
#ifndef PATH_UTIL_HPP
#define PATH_UTIL_HPP

#include <Geode/Geode.hpp>
#include "../abstraction/SingletonBase.hpp"

using namespace geode::prelude;

class PathUtil : public SingletonBase<PathUtil>
{
	friend class SingletonBase<PathUtil>;

public:
	std::filesystem::path* getLevelsFolder() { return &levelsFolder; }

private:
	std::filesystem::path levelsFolder;

private:
	PathUtil()
	{
		levelsFolder = Mod::get()->getSaveDir() / "levels";
	}
};

#endif // PATH_UTIL_HPP