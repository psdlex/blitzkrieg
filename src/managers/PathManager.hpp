#pragma once
#ifndef PATH_MANAGER_HPP
#define PATH_MANAGER_HPP

#include "../abstraction/SingletonBase.hpp"
#include "abstraction/Initializable.hpp"
#include "geode/Geode.hpp"

using namespace geode::prelude;

namespace managers {
    class PathManager : public SingletonBase<PathManager>, public Initializable {
        friend class SingletonBase<PathManager>;

    private:
        static std::filesystem::path m_levelsFolder;

    public:
        Result<> init() override;
        std::filesystem::path* getLevelsFolder() { return &m_levelsFolder; }
    };

    inline std::filesystem::path PathManager::m_levelsFolder;

    inline Result<> PathManager::init()
    {
        m_levelsFolder = Mod::get()->getSaveDir() / "levels";
        return Ok();
    }
};

#endif // PATH_MANAGER_HPP