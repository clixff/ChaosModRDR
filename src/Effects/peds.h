#pragma once

#include "effect.h"
#include <vector>

Ped SpawnPedAroundPlayer(Hash skinModel);

class EffectSpawnSoldier : public Effect
{
public:
	EffectSpawnSoldier()
	{
		ID = "spawn_soldier";
		name = "Spawn Soldier Companion";
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
		name = "Spawn Drunkard Jon";
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
		name = "Spawn Companion Lenny";
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
		name = "Spawn Chicken Companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectKidnapping : public Effect
{
public:
	EffectKidnapping()
	{
		ID = "kidnapping";
		name = "Pig Farmers Kidnap Player";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnHorse : public Effect
{
public:
	EffectSpawnHorse()
	{
		ID = "spawn_horse";
		name = "Spawn Horse";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnMule : public Effect
{
public:
	EffectSpawnMule()
	{
		ID = "spawn_mule";
		name = "Spawn Mule";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnDonkey : public Effect
{
public:
	EffectSpawnDonkey()
	{
		ID = "spawn_donkey";
		name = "Spawn Donkey";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnMiniDonkey : public Effect
{
public:
	EffectSpawnMiniDonkey()
	{
		ID = "spawn_mini_donkey";
		name = "Spawn Mini Donkey";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnGiantDonkey : public Effect
{
public:
	EffectSpawnGiantDonkey()
	{
		ID = "spawn_giant_donkey";
		name = "Spawn Giant Donkey";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnSerialKiller : public Effect
{
public:
	EffectSpawnSerialKiller()
	{
		ID = "spawn_serial_killer";
		name = "Spawn Serial Killer";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnVampire : public Effect
{
public:
	EffectSpawnVampire()
	{
		ID = "spawn_vampire";
		name = "Spawn Vampire";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnGiantCop : public Effect
{
public:
	EffectSpawnGiantCop()
	{
		ID = "spawn_giant_cop";
		name = "Spawn Giant Cop";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


std::vector<Ped> GetNearbyPeds(int32_t Max);