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


class EffectHonor1 : public Effect
{
public:
	EffectHonor1()
	{
		ID = "honor_1";
		name = "honor_1";
		bTimed = false;
	}

	virtual void OnActivate() override;
};

class EffectHonor2 : public Effect
{
public:
	EffectHonor2()
	{
		ID = "honor_2";
		name = "honor_2";
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
