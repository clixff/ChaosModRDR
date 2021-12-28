#pragma once
#include <natives.h>
#include <types.h>
#include <enums.h>
#include <main.h>
#include <string>
#include <stdint.h>
#include <vector>
#include <windows.h>

struct NearbyEntities
{
	int32_t size;
	int64_t entities[100];
};

void LoadModel(Hash model);



void InitWeaponHashes();

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
};

class TestEffect : public Effect
{
public:
	TestEffect()
	{
		ID = "test_effect";
		name = "Test Effect";
		bTimed = false;
	}
};

class TestEffectTimed : public Effect
{
public:
	TestEffectTimed()
	{
		ID = "test_effect_timed";
		name = "Test Effect Timed";
		bTimed = true;
		EffectDuration = 30;
	}
};

