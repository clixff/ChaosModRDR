#pragma once

#include "effect.h"

void TeleportPlayerTo(float X, float Y, float Z);

class EffectLaunchPlayerUp : public Effect
{
public:
	EffectLaunchPlayerUp()
	{
		ID = "launch_player_up";
		name = "Launch Player Up";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectToTheStars : public Effect
{
public:
	EffectToTheStars()
	{
		ID = "to_the_stars";
		name = "To The Stars";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectTeleportToDesert : public Effect
{
public:
	EffectTeleportToDesert()
	{
		ID = "tp_to_desert";
		name = "Teleport To the Desert";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectTeleportToValentine : public Effect
{
public:
	EffectTeleportToValentine()
	{
		ID = "tp_to_valentine";
		name = "Teleport To Valentine";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectTeleportToSaintDenis : public Effect
{
public:
	EffectTeleportToSaintDenis()
	{
		ID = "tp_to_saint_denis";
		name = "Teleport To Saint Denis";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectTeleportToSnowyMountains : public Effect
{
public:
	EffectTeleportToSnowyMountains()
	{
		ID = "tp_to_snowy_mountains";
		name = "Teleport To the Snowy Mountains";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectTeleportToPrison : public Effect
{
public:
	EffectTeleportToPrison()
	{
		ID = "tp_to_prison";
		name = "Teleport To the Prison";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectGivePlayerMoney : public Effect
{
public:
	EffectGivePlayerMoney()
	{
		ID = "give_player_money";
		name = "Give the player 300$";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectBankruptcy : public Effect
{
public:
	EffectBankruptcy()
	{
		ID = "bankruptcy";
		name = "Bankruptcy";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectGiveRifle : public Effect
{
public:
	EffectGiveRifle()
	{
		ID = "give_rifle";
		name = "Give the player a rifle";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectGiveRevolver : public Effect
{
public:
	EffectGiveRevolver()
	{
		ID = "give_revolver";
		name = "Give the player a revolver";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectNoAmmo : public Effect
{
public:
	EffectNoAmmo()
	{
		ID = "no_ammo";
		name = "No ammo";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectRemoveAllWeapons : public Effect
{
public:
	EffectRemoveAllWeapons()
	{
		ID = "remove_all_weapons";
		name = "Remove all player weapons";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectHonorGood : public Effect
{
public:
	EffectHonorGood()
	{
		ID = "honor_good";
		name = "Set Good Honor";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectHonorBad : public Effect
{
public:
	EffectHonorBad()
	{
		ID = "honor_bad";
		name = "Set Bad Honor";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectHonorReset : public Effect
{
public:
	EffectHonorReset()
	{
		ID = "honor_reset";
		name = "Reset Player's Honor";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectDropWeapon : public Effect
{
public:
	EffectDropWeapon()
	{
		ID = "drop_weapon";
		name = "Drop current weapon";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectHealPlayer : public Effect
{
public:
	EffectHealPlayer()
	{
		ID = "heal_player";
		name = "Heal Player";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectAlmostDead : public Effect
{
public:
	EffectAlmostDead()
	{
		ID = "almost_dead";
		name = "Almost Dead";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectRestoreStamina : public Effect
{
public:
	EffectRestoreStamina()
	{
		ID = "restore_stamina";
		name = "Restore Player Stamina";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectRagdoll : public Effect
{
public:
	EffectRagdoll()
	{
		ID = "ragdoll";
		name = "Ragdoll";
		bTimed = false;
	}

	virtual void OnActivate() override;
};


class EffectGiveRandomWeapon : public Effect
{
public:
	EffectGiveRandomWeapon()
	{
		ID = "give_random_weapon";
		name = "Give Random Weapon";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectSetDrunk : public Effect
{
public:
	EffectSetDrunk()
	{
		ID = "set_drunk";
		name = "Player is Drunk";
		bTimed = true;
		EffectDuration = 30;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
};

class EffectClearPursuit : public Effect
{
public:
	EffectClearPursuit()
	{
		ID = "clear_pursuit";
		name = "Stop Pursuit";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectIncreaseBounty : public Effect
{
public:
	EffectIncreaseBounty()
	{
		ID = "increase_bounty";
		name = "Increase Bounty for Player";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectRemoveCurrentVehicle : public Effect
{
public:
	EffectRemoveCurrentVehicle()
	{
		ID = "remove_current_vehicle";
		name = "Remove Current Vehicle";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectGiveLasso : public Effect
{
public:
	EffectGiveLasso()
	{
		ID = "give_lasso";
		name = "Give Lasso to Player";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectTeleportToVanHorn : public Effect
{
public:
	EffectTeleportToVanHorn()
	{
		ID = "tp_to_van_horn";
		name = "Teleport To Van Horn";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectIgnitePlayer : public Effect
{
public:
	EffectIgnitePlayer()
	{
		ID = "ignite_player";
		name = "Ignite Player";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectKickflip : public Effect
{
public:
	EffectKickflip()
	{
		ID = "kickflip";
		name = "Kickflip";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectLightningWeapons : public Effect
{
public:
	EffectLightningWeapons()
	{
		ID = "lightning_weapons";
		name = "Lightning Weapons";
		bTimed = true;
		EffectDuration = 25;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	virtual void OnTick() override;
private:
	Vector3 lastVec;
};

class IEffectSkinChange : public Effect
{
public:
	IEffectSkinChange()
	{
		bTimed = true;
		EffectDuration = 20;
	}

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

	const char* skinToSet = "A_C_COW";
private:
	uint64_t oldSkin1{};
	uint64_t oldSkin2{};
};


class EffectCowSkin : public IEffectSkinChange
{
public:
	EffectCowSkin()
	{
		ID = "cow_skin";
		name = "Player Is a Cow";
		skinToSet = "A_C_COW";
	}
};

class EffectDwarfSkin : public IEffectSkinChange
{
public:
	EffectDwarfSkin()
	{
		ID = "dwarf_skin";
		name = "Player Is a Dwarf";
		skinToSet = "CS_Magnifico";
	}
};

class EffectPigSkin : public IEffectSkinChange
{
public:
	EffectPigSkin()
	{
		ID = "pig_skin";
		name = "Player Is a Pig";
		skinToSet = "A_C_Pig_01";
	}
};

class EffectRatSkin : public IEffectSkinChange
{
public:
	EffectRatSkin()
	{
		ID = "rat_skin";
		name = "Player Is a Rat";
		skinToSet = "A_C_Rat_01";
	}
};

class EffectTurtleSkin : public IEffectSkinChange
{
public:
	EffectTurtleSkin()
	{
		ID = "turtle_skin";
		name = "Player Is a Turtle";
		skinToSet = "A_C_TurtleSnapping_01";
		EffectDuration = 40;
	}
};

void SetPlayerModel(const char* model, uint64_t* ptr1_val, uint64_t* ptr2_val);
void ResetPlayerModel(uint64_t ptr1_val, uint64_t ptr2_val);
