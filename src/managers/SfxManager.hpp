#pragma once
#ifndef SFX_MANAGER_HPP
#define SFX_MANAGER_HPP

#include <Geode/Geode.hpp>
#include "../abstraction/SingletonBase.hpp"
#include "abstraction/Initializable.hpp"
#include "SettingsManager.hpp"
#include "LogManager.hpp"
#include "../defines/Sfxs.hpp"

using namespace geode::prelude;

namespace managers {
    #define SFX_PLAY_PROGRESS_PASSED SfxManager::get()->playProgressPassed
    #define SFX_PLAY_STAGE_PASSED SfxManager::get()->playStagePassed
    #define SFX_SHAREDENGINE FMODAudioEngine::sharedEngine

    class SfxManager : public SingletonBase<SfxManager>, public Initializable {
        friend class SingletonBase<SfxManager>;

    private:
        SfxSettings m_sfxSettings;
        FMOD::ChannelGroup* m_sfxChannelGroup;
        FMOD::Channel* m_sfxChannel = nullptr;
        FMOD::Sound* m_progressPassSound;
        FMOD::Sound* m_stagePassSound;

    public:
        Result<> init() {
            m_sfxSettings = SettingsManager::get()->getSfxSettings();
            SettingsManager::get()->listenForSfxSettingsChanged([this](SfxSettings value) { m_sfxSettings = value; });

            initSounds();
            return Ok();
        }


        void playProgressPassed() {
            FMODAudioEngine::sharedEngine()->m_system->playSound(m_progressPassSound, m_sfxChannelGroup, false, &m_sfxChannel);
            m_sfxChannel->setVolume(m_sfxSettings.m_volume);
        }

        void playStagePassed() {
            FMODAudioEngine::sharedEngine()->m_system->playSound(m_stagePassSound, m_sfxChannelGroup, false, &m_sfxChannel);
            m_sfxChannel->setVolume(m_sfxSettings.m_volume);
        }

    private:
        void initSounds() {
            FMODAudioEngine::sharedEngine()->m_system->createChannelGroup("m_sfxChannelGroup", &m_sfxChannelGroup);
            std::string progressPass, stagePass;
            auto resDir = Mod::get()->getResourcesDir();

            if (m_sfxSettings.m_useCustom) {
                progressPass = m_sfxSettings.m_progressSfxPath.string();
                stagePass = m_sfxSettings.m_stageSfxPath.string();
            } else {
                progressPass = (resDir / SFX_PROGRESS_PASS_SOUND).string();
                stagePass = (resDir / SFX_STAGE_PASS_SOUND).string();
            }

            if (FMODAudioEngine::sharedEngine()->m_system->createSound(progressPass.c_str(), FMOD_DEFAULT, nullptr, &m_progressPassSound) == FMOD_OK) {
                m_progressPassSound->setMode(FMOD_LOOP_OFF);
            }
            
            if (FMODAudioEngine::sharedEngine()->m_system->createSound(stagePass.c_str(), FMOD_DEFAULT, nullptr, &m_stagePassSound) == FMOD_OK) {
                m_stagePassSound->setMode(FMOD_LOOP_OFF);
            }
        }
    };
};

#endif // SFX_MANAGER_HPP