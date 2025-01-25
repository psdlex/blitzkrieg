#pragma once
#ifndef BK_PAUSE_LAYER_HPP
#define BK_PAUSE_LAYER_HPP

#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../objects/LevelProgression.hpp"

class $modify(BKPauseLayer, PauseLayer)
{
	void customSetup();
	void createOrLoadProgressTableButton();
	void createNewProgression();
	void showProgression(LevelProgression* levelProgression);

	void onShowProgressTable(CCObject*);
};

#endif // BK_PAUSE_LAYER_HPP