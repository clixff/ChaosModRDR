#pragma once

#include "effect.h"
#include <set>

class EffectSpawnHotchkissCannon : public Effect
{
public:
	EffectSpawnHotchkissCannon()
	{
		ID = "spawn_hotchkiss_cannon";
		name = "Spawn Hotchkiss cannon";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectTeleportEverything : public Effect
{
public:
	EffectTeleportEverything()
	{
		ID = "teleport_everyithing";
		name = "Teleport Everything To Player";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectSnowstorm : public Effect
{
public:
	EffectSnowstorm()
	{
		ID = "snowstorm";
		name = "Snowstorm";
		bTimed = true;
		EffectDuration = 45;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
};



class EffectThunderstorm : public Effect
{
public:
	EffectThunderstorm()
	{
		ID = "thunderstorm";
		name = "Thunderstorm";
		bTimed = true;
		EffectDuration = 45;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
};

class EffectGiantPeds : public Effect
{
public:
	EffectGiantPeds()
	{
		ID = "giant_peds";
		name = "All Peds Are Giants";
		bTimed = true;
		EffectDuration = 45;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	virtual void OnTick() override;

	void SetScale(float scale);

private:
	std::set<Ped> giantPeds;
};

class EffectAllPedsWannaKillPlayer : public Effect
{
public:
	EffectAllPedsWannaKillPlayer()
	{
		ID = "peds_wanna_kill_player";
		name = "Peds Wanna Kill Player";
		bTimed = true;
		EffectDuration = 30;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	virtual void OnTick() override;
};


class EffectRagdollEveryone : public Effect
{
public:
	EffectRagdollEveryone()
	{
		ID = "ragdoll_everyone";
		name = "Ragdoll Everyone";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectLaunchPedsUp : public Effect
{
public:
	EffectLaunchPedsUp()
	{
		ID = "launch_peds_up";
		name = "Launch Nearby Peds Up";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectInvertedGravity : public Effect
{
public:
	EffectInvertedGravity()
	{
		ID = "inverted_gravity";
		name = "Inverted Gravity";
		bTimed = true;
		EffectDuration = 10;
	}

	virtual void OnActivate() override;
	virtual void OnTick() override;
private:

	std::set<Entity> entities;
};

class EffectLightnings : public Effect
{
public:
	EffectLightnings()
	{
		ID = "lightnings";
		name = "Lightnings Everywhere";
		bTimed = true;
		EffectDuration = 35;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	virtual void OnTick() override;
};