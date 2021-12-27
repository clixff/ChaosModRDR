#pragma once

#include "effect.h"

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