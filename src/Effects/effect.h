#pragma once
#include <natives.h>
#include <types.h>
#include <enums.h>
#include <main.h>
#include <string>
#include <stdint.h>
#include <windows.h>

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

