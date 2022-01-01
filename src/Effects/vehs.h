#pragma once

#include "effect.h"
#include <vector>
#include <set>

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

class EffectMinecartRain : public Effect
{
public:
	EffectMinecartRain()
	{
		ID = "minecart_rain";
		name = "Minecart Rain";
		bTimed = true;
		EffectDuration = 30;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	virtual void OnTick() override;

private:
	std::vector<Vehicle> vehs;

};

class EffectFullAcceleration : public Effect
{
public:
	EffectFullAcceleration()
	{
		ID = "full_acceleration";
		name = "Full Acceleration";
		bTimed = true;
		EffectDuration = 30;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	virtual void OnTick() override;
private:
	std::vector<Vehicle> vehs;

	std::set<Ped> horses;
};

class EffectEveryoneExitsVehs : public Effect
{
public:
	EffectEveryoneExitsVehs()
	{
		ID = "everyone_exits_vehs";
		name = "Everyone Exits Their Vehicles";
	}

	virtual void OnActivate() override;
};

class EffectSetPlayerIntroRandomVeh : public Effect
{
public:
	EffectSetPlayerIntroRandomVeh()
	{
		ID = "set_to_random_veh";
		name = "Set Player Into Random Vehicle";
	}

	virtual void OnActivate() override;
};

class EffectSpawnHotAirBalloon : public Effect
{
public:
	EffectSpawnHotAirBalloon()
	{
		ID = "spawn_balloon";
		name = "Spawn Hot Air Balloon";
	}

	virtual void OnActivate() override;
};

class EffectIgnitePlayersWagon : public Effect
{
public:
	EffectIgnitePlayersWagon()
	{
		ID = "ignite_wagon";
		name = "Ignite Player's Wagon";
	}

	virtual void OnActivate() override;
};

class EffectIgniteNearbyWagons : public Effect
{
public:
	EffectIgniteNearbyWagons()
	{
		ID = "ignite_nearby_wagons";
		name = "Ignite Nearby Wagons";
	}

	virtual void OnActivate() override;
};

class EffectSpawnCanoe : public Effect
{
public:
	EffectSpawnCanoe()
	{
		ID = "spawn_canoe";
		name = "Spawn Canoe";
	}

	virtual void OnActivate() override; 
};
 
class EffectHorsesRain : public Effect
{
public:
	EffectHorsesRain()
	{
		ID = "horses_rain";
		name = "Horses Rain";
		bTimed = true;
		EffectDuration = 25;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	virtual void OnTick() override;

private:
	std::vector<Ped> horses;

};
