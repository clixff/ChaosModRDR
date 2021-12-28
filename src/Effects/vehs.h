#pragma once

#include "effect.h"
#include <vector>

std::vector<Vehicle> GetNearbyVehs(int32_t Max);

class EffectSpawnWagon : public Effect
{
public:
	EffectSpawnWagon()
	{
		ID = "spawn_wagon";
		name = "Spawn Coach";
	}
	
	virtual void OnActivate() override;
};

class EffectFlipAllVehs : public Effect
{
public:
	EffectFlipAllVehs()
	{
		ID = "flip_vehs";
		name = "Flip All Vehicles";
	}

	virtual void OnActivate() override;
};