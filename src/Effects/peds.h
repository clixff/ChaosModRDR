#pragma once

#include "effect.h"
#include <vector>
#include <set>

Ped SpawnPedAroundPlayer(Hash skinModel, bool bSetInVehicle = true, bool bSpawnHorseForPed = false);

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

class EffectSpawnBearCompanion : public Effect
{
public:
	EffectSpawnBearCompanion()
	{
		ID = "spawn_bear_companion";
		name = "Spawn Bear Companion";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnAngryCorpse : public Effect
{
public:
	EffectSpawnAngryCorpse()
	{
		ID = "spawn_angry_corpse";
		name = "Spawn Angry Corpse";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectSpawnAngryCaveman : public Effect
{
public:
	EffectSpawnAngryCaveman()
	{
		ID = "spawn_angry_caveman";
		name = "Spawn Angry Caveman";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnAngryTwin : public Effect
{
public:
	EffectSpawnAngryTwin()
	{
		ID = "spawn_angry_twin";
		name = "Spawn Angry Twin";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnAngryCowboy : public Effect
{
public:
	EffectSpawnAngryCowboy()
	{
		ID = "spawn_angry_cowboy";
		name = "Spawn Angry Cowboy";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnUndeadBoss : public Effect
{
public:
	EffectSpawnUndeadBoss()
	{
		ID = "spawn_undead_boss";
		name = "Spawn Undead Boss";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSpawnGrieferMicah : public Effect
{
public:
	EffectSpawnGrieferMicah()
	{
		ID = "spawn_greifer_micah";
		name = "Spawn Griefer Micah";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectDutchStealsPlayersVeh : public Effect
{
public:
	EffectDutchStealsPlayersVeh()
	{
		ID = "dutch_steals_veh";
		name = "Dutch Steals Player's Veh";
		bTimed = true;
		EffectDuration = 20;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
private:
	Ped dutch;
};

class EffectSpawnPredator : public Effect
{
public:
	EffectSpawnPredator()
	{
		ID = "spawn_predator";
		name = "Spawn Predator";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectPedsBhop : public Effect
{
public:
	EffectPedsBhop()
	{
		ID = "peds_bhop";
		name = "Peds Can't Stop Bunny Hopping";
		bTimed = true;
		EffectDuration = 25;
	}

	virtual void OnTick() override;
};

class EffectPedsSpin : public Effect
{
public:
	EffectPedsSpin()
	{
		ID = "peds_spin";
		name = "Spinning Peds";
		bTimed = true;
		EffectDuration = 30;
	}

	virtual void OnActivate() override;
	virtual void OnTick() override;
private:
	std::set<Ped> peds;

	float heading = 0.0f;
};

class EffectCloneEnemy : public Effect
{
public:
	EffectCloneEnemy()
	{
		ID = "clone_enemy";
		name = "Clone Random Enemy";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectPedsFollowPlayer : public Effect
{
public:
	EffectPedsFollowPlayer()
	{
		ID = "peds_follow_player";
		name = "Peds Follow Player";
		bTimed = true;
		EffectDuration = 30;
	}

	virtual void OnActivate() override;
	virtual void OnTick() override;
	virtual void OnDeactivate() override;
private:
	std::set<Ped> peds;
};

class EffectPedsFleeing : public Effect
{
public:
	EffectPedsFleeing()
	{
		ID = "peds_fleeing";
		name = "Nearby Peds Are Fleeing";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

std::vector<Ped> GetNearbyPeds(int32_t Max);

void RemoveAllPedWeapons(Ped ped);

void MarkPedAsCompanion(Ped ped);
void MarkPedAsEnemy(Ped ped);