#pragma once

#include <natives.h>
#include <types.h>
#include <enums.h>
#include <main.h>
#include <string>
#include <stdint.h>
#include <vector>
#include <map>
#include <windows.h>

#define GET_HASH(str) GAMEPLAY::GET_HASH_KEY((char*)str)

#define M_PI    3.14159265358979323846

struct LinearColor
{
	LinearColor()
	{
	};
	
	LinearColor(uint8_t _R, uint8_t _G, uint8_t _B, uint8_t _A)
	{
		R = _R;
		G = _G;
		B = _B;
		A = _A;
	};
	
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
	uint8_t A = 255;
};


template<typename T>
inline T Lerp(T min, T max, double alpha)
{
	return min + (T) (double(max - min) * alpha);
}

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
	uint32_t DisplayTime = 0;
	
	static std::vector<const char*> WeaponNames;
	static std::vector <Hash> WeaponHashes;
	
	static std::vector<const char*> weatherNames;
	static std::vector <Hash> weatherHashes;
	
	/**
	 * Adds delta time to the current internal timer.
	 * Returns whether time (maxMs) has expired or not
	 */
	bool TimerTick(uint32_t maxMs);
	
	bool TimerTick(uint32_t maxMs, uint32_t timerIndex);
	
	bool bIsMeta = false;
	
	bool bIsFake = false;
private:
	std::map <uint32_t, uint32_t> internalTimer = {};
};

class MetaEffect : public Effect
{
public:
	MetaEffect() : Effect()
	{
		bIsMeta = true;
		bTimed = true;
	}

public:
	bool bEnabled = true;
};