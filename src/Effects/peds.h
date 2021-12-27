#pragma once

#include "effect.h"


class EffectSpawnSoldier : public Effect
{
public:
	EffectSpawnSoldier()
	{
		ID = "spawn_soldier";
		name = "Spawn soldier companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnDrunkardJon : public Effect
{
public:
	EffectSpawnDrunkardJon()
	{
		ID = "spawn_jon";
		name = "Spawn drunkard Jon";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnLenny : public Effect
{
public:
	EffectSpawnLenny()
	{
		ID = "spawn_lenny";
		name = "Spawn companion Lenny";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnChicken : public Effect
{
public:
	EffectSpawnChicken()
	{
		ID = "spawn_chicken";
		name = "Spawn chicken companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};