#pragma once
#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <Geode/Geode.hpp>
#include "../abstraction/SingletonBase.hpp"
#include "../include/EnumBitmask.hpp"
#include "abstraction/Initializable.hpp"
#include "SettingsManager.hpp"

using namespace geode::prelude;

namespace managers
{
    class LogManager : public SingletonBase<LogManager>, public Initializable
    {
        friend class SingletonBase<LogManager>;

    private:
        DebugSettings m_debugSettings;

    public:

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

inline Result<> managers::LogManager::init()
{
    m_debugSettings = SettingsManager::get()->getDebugSettings();
    return Ok();
}

template<typename... Args>
inline void managers::LogManager::debug(geode::log::impl::FmtStr<Args...> str, Args&&... args)
{
    if ((m_debugSettings & DSFlags::ShowDebug) != DSFlags::None) {
        log::debug(str, args...);
    }
}

template<typename... Args>
inline void managers::LogManager::info(geode::log::impl::FmtStr<Args...> str, Args&&... args)
{
    if ((m_debugSettings & DSFlags::ShowDebug) != DSFlags::None && 
        (m_debugSettings & DSFlags::ShowInfo) != DSFlags::None) {
        log::debug(str, args...);
    }
}

template<typename... Args>
inline void managers::LogManager::warn(geode::log::impl::FmtStr<Args...> str, Args&&... args)
{
    if ((m_debugSettings & DSFlags::ShowDebug) != DSFlags::None && 
        (m_debugSettings & DSFlags::ShowWarns) != DSFlags::None) {
        log::debug(str, args...);
    }
}

template<typename... Args>
inline void managers::LogManager::error(geode::log::impl::FmtStr<Args...> str, Args&&... args)
{
    if ((m_debugSettings & DSFlags::ShowDebug) != DSFlags::None && 
        (m_debugSettings & DSFlags::ShowErrors) != DSFlags::None) {
        log::debug(str, args...);
    }
}

#endif // LOG_MANAGER_HPP