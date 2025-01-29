#pragma once
#ifndef GENERIC_SERIALIZERS_HPP
#define GENERIC_SERIALIZERS_HPP

#include <Geode/Geode.hpp>
#include "../objects/LevelProgression.hpp"

using namespace geode::prelude;

// ProgressBestRun
template<>
struct matjson::Serialize<ProgressBestRun>
{
	static Result<ProgressBestRun> fromJson(const matjson::Value& value)
	{
		ProgressBestRun info;
		GEODE_UNWRAP_INTO(info.m_fromPercent, value["fromPercent"].as<double>());
		GEODE_UNWRAP_INTO(info.m_toPercent, value["toPercent"].as<double>());

		return Ok(info);
	}

	static matjson::Value toJson(const ProgressBestRun& value)
	{
		auto obj = matjson::makeObject({
			{ "fromPercent", value.m_fromPercent },
			{ "toPercent", value.m_toPercent }
		});

		return obj;
	}
};

// ProgressInfo
template<>
struct matjson::Serialize<ProgressInfo>
{
	static Result<ProgressInfo> fromJson(const matjson::Value& value)
	{
		ProgressInfo info;
		GEODE_UNWRAP_INTO(info.m_isPassed, value["passed"].as<bool>());
		GEODE_UNWRAP_INTO(info.m_fromPercent, value["fromPercent"].as<uint32_t>());
		GEODE_UNWRAP_INTO(info.m_toPercent, value["toPercent"].as<uint32_t>());
		GEODE_UNWRAP_INTO(info.m_passAmount, value["passAmount"].as<uint32_t>());
		GEODE_UNWRAP_INTO(info.m_attemptsToPass, value["attemptsToPass"].as<uint32_t>());
		GEODE_UNWRAP_INTO(info.m_bestRun, value["bestRun"].as<ProgressBestRun>());

		return Ok(info);
	}

	static matjson::Value toJson(const ProgressInfo& value)
	{
		auto obj = matjson::makeObject({
			{ "passed", value.m_isPassed },
			{ "fromPercent", value.m_fromPercent },
			{ "toPercent", value.m_toPercent },
			{ "attemptsToPass", value.m_attemptsToPass },
			{ "passAmount", value.m_passAmount },
			{ "bestRun", value.m_bestRun }
		});

		return obj;
	}
};

// ProgressionStage
template<>
struct matjson::Serialize<ProgressionStage>
{
	static Result<ProgressionStage> fromJson(const matjson::Value& value)
	{
		ProgressionStage stage;
		GEODE_UNWRAP_INTO(stage.m_stage, value["stage"].as<uint32_t>());
		GEODE_UNWRAP_INTO(stage.m_isActive, value["active"].as<bool>());
		GEODE_UNWRAP_INTO(stage.m_isPassed, value["passed"].as<bool>());
		GEODE_UNWRAP_INTO(stage.m_isAvailable, value["available"].as<bool>());
		GEODE_UNWRAP_INTO(stage.m_progresses, value["progresses"].as<std::vector<ProgressInfo>>());

		return Ok(stage);
	}

	static matjson::Value toJson(const ProgressionStage& value)
	{
		auto obj = matjson::makeObject({
			{ "stage", value.m_stage },
			{ "passed", value.m_isPassed },
			{ "available", value.m_isAvailable },
			{ "active", value.m_isActive },
			{ "progresses", value.m_progresses }
		});

		return obj;	
	}
};

// LevelProgression
template<>
struct matjson::Serialize<LevelProgression>
{
	static Result<LevelProgression> fromJson(const matjson::Value& value)
	{
		LevelProgression progression;
		GEODE_UNWRAP_INTO(progression.m_stages, value["stages"].as<std::vector<ProgressionStage>>());

		return Ok(progression);
	}

	static matjson::Value toJson(const LevelProgression& value)
	{
		auto obj = matjson::makeObject({
			{ "stages", value.m_stages }
		});

		return obj;	
	}
};

#endif // GENERIC_SERIALIZERS_HPP