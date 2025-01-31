#pragma once
#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#define LMDEBUG managers::LogManager::get()->debug
#define LMINFO managers::LogManager::get()->info
#define LMWARN managers::LogManager::get()->warn
#define LMERROR managers::LogManager::get()->error

#include <Geode/Geode.hpp>
#include "../abstraction/SingletonBase.hpp"
#include "../include/EnumBitmask.hpp"
#include "abstraction/Initializable.hpp"
#include "SettingsManager.hpp"

using namespace geode::prelude;

namespace managers {
    class LogManager : public SingletonBase<LogManager>, public Initializable {
        friend class SingletonBase<LogManager>;

    private:
        DebugSettings m_debugSettings;

    public:
        Result<> LogManager::init() override;

        template<typename... Args>
        void debug(geode::log::impl::FmtStr<Args...> str, Args&&... args);

        template<typename... Args>
        void info(geode::log::impl::FmtStr<Args...> str, Args&&... args);

        template<typename... Args>
        void warn(geode::log::impl::FmtStr<Args...> str, Args&&... args);

        template<typename... Args>
        void error(geode::log::impl::FmtStr<Args...> str, Args&&... args);
    };
};

inline Result<> managers::LogManager::init() {
    m_debugSettings = SettingsManager::get()->getDebugSettings();
    SettingsManager::get()->listenForDebugSettingsChanged([this](auto flags) { m_debugSettings = flags; });
    return Ok();
}

template<typename... Args>
inline void managers::LogManager::debug(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    if ((m_debugSettings & DSFlags::Descriptive) != DSFlags::Disabled) {
        log::debug(str, args...);
    }
}

template<typename... Args>
inline void managers::LogManager::info(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    if ((m_debugSettings & DSFlags::Descriptive) != DSFlags::Disabled) {
        log::info(str, args...);
    }
}

template<typename... Args>
inline void managers::LogManager::warn(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    if ((m_debugSettings & DSFlags::ErrorsAndWarns) != DSFlags::Disabled) {
        log::warn(str, args...);
    }
}

template<typename... Args>
inline void managers::LogManager::error(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    if ((m_debugSettings & DSFlags::ErrorsAndWarns) != DSFlags::Disabled) {
        log::error(str, args...);
    }
}

#endif // LOG_MANAGER_HPP