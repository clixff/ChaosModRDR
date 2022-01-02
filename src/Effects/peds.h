#pragma once

#include "effect.h"
#include <vector>
#include <set>

Ped SpawnPedAroundPlayer(Hash skinModel, bool bSetInVehicle = true);

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

class EffectSpawnAngrySkeleton : public Effect
{
public:
	EffectSpawnAngrySkeleton()
	{
		ID = "spawn_skeleton";
		name = "Spawn Angry Skeleton";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnAngryDwarf : public Effect
{
public:
	EffectSpawnAngryDwarf()
	{
		ID = "spawn_dwarf";
		name = "Spawn Angry Dwarf";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnCompanionBertram : public Effect
{
public:
	EffectSpawnCompanionBertram()
	{
		ID = "spawn_bertram";
		name = "Spawn Companion Bertram";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnFrozenCouple : public Effect
{
public:
	EffectSpawnFrozenCouple()
	{
		ID = "spawn_frozen_couple";
		name = "Spawn Frozen Couple";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectSpawnRobot : public Effect
{
public:
	EffectSpawnRobot()
	{
		ID = "spawn_robot";
		name = "Spawn Robot Companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnLassoGuy : public Effect
{
public:
	EffectSpawnLassoGuy()
	{
		ID = "lasso_guy";
		name = "Spawn Lasso Guy";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSkyrimIntro : public Effect
{
public:
	EffectSkyrimIntro()
	{
		ID = "skyrim_intro";
		name = "You're Finally Awake";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnParrotCompanion : public Effect
{
public:
	EffectSpawnParrotCompanion()
	{
		ID = "parrot_companion";
		name = "Spawn Parrot Companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnShireHorse : public Effect
{
public:
	EffectSpawnShireHorse()
	{
		ID = "spawn_shire_horse";
		name = "Spawn Shire Horse";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectUndeadNightmare : public Effect
{
public:
	EffectUndeadNightmare()
	{
		ID = "undead_nightmare";
		name = "Undead Nightmare";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnDogCompanion : public Effect
{
public:
	EffectSpawnDogCompanion()
	{
		ID = "spawn_dog_companion";
		name = "Spawn Dog Companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnCatCompanion : public Effect
{
public:
	EffectSpawnCatCompanion()
	{
		ID = "spawn_cat_companion";
		name = "Spawn Cat Companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

std::vector<Ped> GetNearbyPeds(int32_t Max);

void RemoveAllPedWeapons(Ped ped);