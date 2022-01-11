#pragma once
#include <natives.h>
#include <types.h>
#include <enums.h>
#include <main.h>
#include <string>
#include <stdint.h>
#include <vector>
#include <windows.h>

#define GET_HASH(str) GAMEPLAY::GET_HASH_KEY((char*)str)

#define M_PI	3.14159265358979323846

struct NearbyEntities
{
	int32_t size;
	int64_t entities[100];
};

void LoadModel(Hash model);

void InitWeaponHashes();

void InitWeatherHashes();

class Effect
{
public:
	Effect();
	~Effect();

	virtual void OnActivate();
	virtual void OnDeactivate();

	virtual void OnTick();

	std::string ID = "";
	std::string name = "";

	bool bTimed = false;

	/** In seconds */
	uint32_t EffectDuration = 0;

	uint32_t DeactivationTime = 0;
	uint32_t ActivationTime = 0;

	static std::vector<const char*> WeaponNames;
	static std::vector<Hash> WeaponHashes;

	static std::vector<const char*> weatherNames;
	static std::vector<Hash> weatherHashes;
};