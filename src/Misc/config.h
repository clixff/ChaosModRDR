#pragma once

#include "../pch.h"
#include <string>
#include <vector>
#include <filesystem>

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

	bool bTwitch = false;

	std::vector<ConfigEffect> effects;


public:
	static std::filesystem::path GetFilePath();
};