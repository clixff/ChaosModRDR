#pragma once

#include "effect.h"

class EffectLaunchPlayerUp : public Effect
{
public:
	EffectLaunchPlayerUp()
	{
		ID = "launch_player_up";
		name = "Launch player up";
		bTimed = false;
	}

	virtual void OnActivate() override;
};