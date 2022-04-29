#pragma once

#include "../pch.h"
#include <string>
#include <vector>
#include <filesystem>

struct ConfigControls
{
	uint8_t activateMod = VK_F7;
	uint8_t toggleEffects = VK_F8;
	uint8_t testEffect = VK_F10;
	uint8_t disableTwitch = VK_F11;
	uint8_t instaKill = VK_F12;
};

struct ConfigEffect
{
	std::string id;
	std::string name;
	bool bEnabled = true;
	uint32_t duration = 0;
	uint32_t chance = 1;
};

class Config
{
public:
	Config();
	
	~Config();
	
	/** Reads config file and saves data to this object */
	void Read();

public:
	
	uint32_t intervalTime = 60;
	uint32_t votingTime = 30;
	uint32_t metaInterval = 900;
	uint32_t effectDisplayTime = 15;
	
	bool bTwitch = false;
	bool bSubs = false;
	bool bSingleShotSub = false;
	
	std::vector <ConfigEffect> effects;
	
	ConfigControls controls;
public:
	static std::filesystem::path GetFilePath();
	
};