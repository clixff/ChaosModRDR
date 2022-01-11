#include "config.h"
#include <fstream>
#include "../script.h"

Config::Config()
{
}

Config::~Config()
{
}

void Config::Read()
{
	auto path = Config::GetFilePath();

	if (!std::filesystem::exists(path))
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "Config file not found", "Chaos Mod", MB_OK);
		return;
	}

	std::ifstream fileStream(path);

	std::string fileContent( (std::istreambuf_iterator<char>(fileStream)),
		(std::istreambuf_iterator<char>()) );

	if (!fileContent.size())
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "Failed to parse config.json", "Chaos Mod", MB_OK);
		return;
	}

	rapidjson::Document document;

	document.Parse(fileContent.c_str());

	if (document.HasParseError())
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, "Failed to parse config.json", "Chaos Mod", MB_OK);
		return;
	}

	auto* chaosMod = ChaosMod::Get();

	if (!chaosMod)
	{
		return;
	}

	if (document.HasMember("interval"))
	{
		intervalTime = document["interval"].GetUint();
		chaosMod->effectsInterval = intervalTime;
	}

	if (document.HasMember("votingDuration"))
	{
		votingTime = document["votingDuration"].GetUint();
		chaosMod->effectsVoteTime = votingTime;
	}

	if (document.HasMember("twitch"))
	{
		bTwitch = document["twitch"].GetBool();
	}

	if (!document.HasMember("effects"))
	{
		return;
	}

	this->effects.clear();

	auto effectsList = document["effects"].GetArray();

	for (auto& effect : effectsList)
	{
		ConfigEffect configEffect;

		if (!effect.IsObject() || !effect.HasMember("id"))
		{
			continue;
		}

		configEffect.id = effect["id"].GetString();
		configEffect.name = effect["name"].GetString();
		configEffect.bEnabled = effect["enabled"].GetBool();

		if (effect.HasMember("chance"))
		{
			configEffect.chance = effect["chance"].GetUint();
			
			if (configEffect.chance > 10)
			{
				configEffect.chance = 10;
			}
			else if (configEffect.chance < 1)
			{
				configEffect.chance = 1;
			}
		}

		if (effect.HasMember("duration"))
		{
			configEffect.duration = effect["duration"].GetUint();
		}

		effects.push_back(configEffect);

		if (!configEffect.bEnabled)
		{
			continue;
		}

		auto* effect = chaosMod->EffectsMap[configEffect.id];

		if (effect)
		{
			if (configEffect.name.size())
			{
				effect->name = configEffect.name;
			}
			
			if (configEffect.duration)
			{
				effect->EffectDuration = configEffect.duration;
			}
		}
		
	}
}

std::filesystem::path Config::GetFilePath()
{
	return std::filesystem::current_path() / L"ChaosMod" / L"config.json";
}
