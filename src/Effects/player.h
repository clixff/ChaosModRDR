#pragma once

#include "effect.h"

void TeleportPlayerTo(float X, float Y, float Z);

void DisableAllMovements();

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

class EffectGivePlayerMoney : public Effect
{
public:
	EffectGivePlayerMoney()
	{
		ID = "give_player_money";
		name = "Give 300$ to Player";
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
		name = "Give a Rifle to Player";
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
		name = "Give a Revolver To Player";
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
		name = "Remove All Player Weapons";
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
	
	virtual void OnTick() override;
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
	
	virtual void OnActivate() override;
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

class EffectPlayerSleep : public IEffectSkinChange
{
public:
	EffectPlayerSleep()
	{
		ID = "player_sleep";
		name = "I Need Some Sleep";
		bTimed = true;
		EffectDuration = 15;
	}
	
	virtual void OnActivate() override;
};

class EffectPlayerIsMinion : public IEffectSkinChange
{
public:
	EffectPlayerIsMinion()
	{
		ID = "player_minion";
		name = "Player Is Minion";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnTick() override;
	
	virtual void OnDeactivate() override;
};


class EffectTeleportToWaypoint : public IEffectSkinChange
{
public:
	EffectTeleportToWaypoint()
	{
		ID = "tp_to_waypoint";
		name = "Teleport To Waypoint";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

void SetPlayerModel(const char* model, uint64_t* ptr1_val, uint64_t* ptr2_val);

void ResetPlayerModel(uint64_t ptr1_val, uint64_t ptr2_val);

class EffectExplosiveWeapons : public Effect
{
public:
	EffectExplosiveWeapons()
	{
		ID = "explosive_weapons";
		name = "Explosive Weapons";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
	
	virtual void OnTick() override;

private:
	Vector3 lastVec;
};

class EffectTeleportWeapons : public Effect
{
public:
	EffectTeleportWeapons()
	{
		ID = "teleport_weapons";
		name = "Teleportation Weapons";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
	
	virtual void OnTick() override;

private:
	Vector3 lastVec;
};


class EffectBloodTrails : public Effect
{
public:
	EffectBloodTrails()
	{
		ID = "blood_trails";
		name = "Blood Trails";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnTick() override;
};


class EffectSetRandomWalkStyle : public Effect
{
public:
	EffectSetRandomWalkStyle()
	{
		ID = "random_walk_style";
		name = "Set Random Walk Style";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
};

class EffectGiveSniperRifle : public Effect
{
public:
	EffectGiveSniperRifle()
	{
		ID = "give_sniper_rifle";
		name = "Give a Sniper Rifle To Player";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectGiveDynamite : public Effect
{
public:
	EffectGiveDynamite()
	{
		ID = "give_dynamite";
		name = "Give a Dynamite To Player";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectThrowingKnives : public Effect
{
public:
	EffectThrowingKnives()
	{
		ID = "give_knives";
		name = "Give Throwing Knives To Player";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectTeleportFewMeters : public Effect
{
public:
	EffectTeleportFewMeters()
	{
		ID = "tp_few_meters";
		name = "Teleport Player a Few Meters";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};


class EffectBlackingOut : public Effect
{
public:
	EffectBlackingOut()
	{
		ID = "blacking_out";
		name = "Blacking Out";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectRandomClothes : public Effect
{
public:
	EffectRandomClothes()
	{
		ID = "random_clothes";
		name = "Set Random Clothes";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectMostWanted : public Effect
{
public:
	EffectMostWanted()
	{
		ID = "most_wanted";
		name = "Most Wanted";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectSetWinterOutfit : public Effect
{
public:
	EffectSetWinterOutfit()
	{
		ID = "winter_outfit";
		name = "Set Winter Outfit";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectSuperJump : public Effect
{
public:
	EffectSuperJump()
	{
		ID = "super_jump";
		name = "Super Jump";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnTick() override;
};

class EffectDisableLeftRight : public Effect
{
public:
	EffectDisableLeftRight()
	{
		ID = "disable_left_right";
		name = "Disable Left & Right Movements";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnTick() override;
};

class EffectDisableForwardBackward : public Effect
{
public:
	EffectDisableForwardBackward()
	{
		ID = "disable_forward_backward";
		name = "Disable Forward & Backward Movements";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnTick() override;
};

class EffectDisableSprintJump : public Effect
{
public:
	EffectDisableSprintJump()
	{
		ID = "disable_sprint_jump";
		name = "Disable Sprint & Jump";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnTick() override;
};

class EffectDisableAllMovements : public Effect
{
public:
	EffectDisableAllMovements()
	{
		ID = "disable_movements";
		name = "Disable All Movements";
		bTimed = true;
		EffectDuration = 20;
	}
	
	virtual void OnTick() override;
};


class EffectDisableAttackButton : public Effect
{
public:
	EffectDisableAttackButton()
	{
		ID = "disable_attack";
		name = "Disable Attack Button";
		bTimed = true;
		EffectDuration = 20;
	}
	
	virtual void OnTick() override;
};

class EffectDisableAiming : public Effect
{
public:
	EffectDisableAiming()
	{
		ID = "disable_aiming";
		name = "Disable Aiming";
		bTimed = true;
		EffectDuration = 20;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
	
	virtual void OnTick() override;
};

class EffectRandomHonor : public Effect
{
public:
	EffectRandomHonor()
	{
		ID = "random_honor";
		name = "Random Honor Change";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectInvertVelocity : public Effect
{
public:
	EffectInvertVelocity()
	{
		ID = "invert_velocity";
		name = "Invert And Increase Current Velocity";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};


class EffectIncreaseVelocity : public Effect
{
public:
	EffectIncreaseVelocity()
	{
		ID = "increase_velocity";
		name = "Increase Current Velocity";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};


class EffectBunnyhop : public Effect
{
public:
	EffectBunnyhop()
	{
		ID = "bunnyhop";
		name = "Bunny Hop";
		bTimed = true;
		EffectDuration = 20;
	}
	
	virtual void OnTick() override;
};


class EffectEyeDisorder : public Effect
{
public:
	EffectEyeDisorder()
	{
		ID = "eye_disorder";
		name = "Eye Disorder";
		bTimed = true;
		EffectDuration = 25;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
};

class EffectBirdSkin : public IEffectSkinChange
{
public:
	EffectBirdSkin()
	{
		ID = "bird_skin";
		name = "Player Is a Bird";
		skinToSet = "A_C_BlueJay_01";
	}
	
	virtual void OnActivate() override;
	
	virtual void OnTick() override;
};

class EffectBodySwap : public Effect
{
public:
	EffectBodySwap()
	{
		ID = "body_swap";
		name = "Body Swap";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;

private:
	Ped clone = 0;
	Hash pedSkin = 0;
};

class EffectPlayerSpin : public Effect
{
public:
	EffectPlayerSpin()
	{
		ID = "player_spin";
		name = "Spin";
		bTimed = true;
		EffectDuration = 20;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnTick() override;

private:
	float heading = 0.0f;
};

class EffectPlayerLosesWeight : public Effect
{
public:
	EffectPlayerLosesWeight()
	{
		ID = "lose_weight";
		name = "Player Loses Weight";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};


class EffectPlayerGainsWeight : public Effect
{
public:
	EffectPlayerGainsWeight()
	{
		ID = "gain_weight";
		name = "Player Gains Weight";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};


class EffectSetRandomHat : public Effect
{
public:
	EffectSetRandomHat()
	{
		ID = "random_hat";
		name = "Set Random Hat";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectGravityGun : public Effect
{
public:
	EffectGravityGun()
	{
		ID = "gravity_gun";
		name = "Gravity Gun";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
	
	virtual void OnTick() override;

private:
	Vector3 lastVec;
};

class EffectDisableDeadEye : public Effect
{
public:
	EffectDisableDeadEye()
	{
		ID = "disable_dead_eye";
		name = "Disable Dead Eye";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
};

class EffectSetRandomVelocity : public Effect
{
public:
	EffectSetRandomVelocity()
	{
		ID = "random_velocity";
		name = "Set Random Velocity";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectFirstPerson : public Effect
{
public:
	EffectFirstPerson()
	{
		ID = "first_person";
		name = "First Person";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnTick() override;
};


class EffectTopDownCamera : public Effect
{
public:
	EffectTopDownCamera()
	{
		ID = "top_down_camera";
		name = "Top Down Camera";
		bTimed = true;
		EffectDuration = 30;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnTick() override;
	
	virtual void OnDeactivate() override;

private:
	Camera cam;
};

class EffectAgitateHorse : public Effect
{
public:
	EffectAgitateHorse()
	{
		ID = "agitate_horse";
		name = "Agitate Player's Horse";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectTpRandomLocation : public Effect
{
public:
	EffectTpRandomLocation()
	{
		ID = "tp_to_random";
		name = "Teleport To Random Location";
		bTimed = false;
	}
	
	virtual void OnActivate() override;
};

class EffectFakeTeleport : public EffectTpRandomLocation
{
public:
	EffectFakeTeleport()
	{
		ID = "fake_teleport";
		name = "Fake Teleport";
		bTimed = true;
		EffectDuration = 20;
		bIsFake = true;
	}
	
	virtual void OnActivate() override;
	
	virtual void OnTick() override;
	
	virtual void OnDeactivate() override;

private:
	Vector3 oldCoord;
	std::string oldName;
	
	bool bTeleportedBack = false;
	
	void TeleportToOldCoord();
};
