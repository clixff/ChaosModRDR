#include "player.h"
#include "peds.h"

void EffectLaunchPlayerUp::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Entity entityToUse = playerPed;

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		entityToUse = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		entityToUse = PED::GET_MOUNT(playerPed);
	}
	else
	{
		PED::SET_PED_TO_RAGDOLL(playerPed, 5000, 5000, 0, true, true, false);
	}

	Vector3 entityVelocity = ENTITY::GET_ENTITY_VELOCITY(entityToUse, 0);

	ENTITY::SET_ENTITY_VELOCITY(entityToUse, entityVelocity.x, entityVelocity.y, 35.0f);
}

void EffectToTheStars::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Entity entityToUse = playerPed;

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		entityToUse = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		entityToUse = PED::GET_MOUNT(playerPed);
	}
	else
	{
		PED::SET_PED_TO_RAGDOLL(playerPed, 10000, 10000, 0, true, true, false);
	}

	Vector3 entityLocation = ENTITY::GET_ENTITY_COORDS(entityToUse, true, 0);

	ENTITY::SET_ENTITY_COORDS(entityToUse, entityLocation.x, entityLocation.y, 800.0f, 0, 0, 0, 0);
}

void TeleportPlayerTo(float X, float Y, float Z)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Entity entityToUse = playerPed;

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		entityToUse = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		entityToUse = PED::GET_MOUNT(playerPed);
	}

	ENTITY::SET_ENTITY_COORDS(entityToUse, X, Y, Z, 0, 0, 0, 0);
}

void EffectTeleportToDesert::OnActivate()
{
	TeleportPlayerTo(-5400.0f, -3500.0f, -20.0f);
}

void EffectTeleportToValentine::OnActivate()
{
	TeleportPlayerTo(-301.0f, 790.0f, 119.0f);
}

void EffectTeleportToSaintDenis::OnActivate()
{
	TeleportPlayerTo(2432.8f, -1216.0f, 46.0f);
}

void EffectTeleportToSnowyMountains::OnActivate()
{
	TeleportPlayerTo(-1505.4f, 2454.2f, 419.5f);
}

void EffectTeleportToPrison::OnActivate()
{
	TeleportPlayerTo(3327.2f, -664.6f, 58.2f);
}

void EffectGivePlayerMoney::OnActivate()
{
	CASH::PLAYER_ADD_CASH(30000, 0x2CD419DC);
}

void EffectBankruptcy::OnActivate()
{
	/** _MONEY_DECREMENT_CASH_BALANCE */
	invoke<Void>(0x466BC8769CF26A7A, 999999999);
}

void EffectGiveRifle::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_RIFLE_SPRINGFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectGiveRevolver::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_REVOLVER_SCHOFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectNoAmmo::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** REMOVE_ALL_PED_AMMO */
	invoke<Void>(0x1B83C0DEEBCBB214, playerPed);

	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_UNARMED");
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, unarmed, 1, 0, 0, 0);
}

void EffectRemoveAllWeapons::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	RemoveAllPedWeapons(playerPed);
}

void EffectDropWeapon::OnActivate()
{

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_UNARMED");
	Hash weaponHash = 0;

	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, 0, 0, 0) && WEAPON::IS_WEAPON_VALID(weaponHash) && weaponHash != unarmed)
	{
		WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(playerPed, weaponHash, 0.0f, 0.0f, 0.0f, 0);
		WEAPON::SET_CURRENT_PED_WEAPON(playerPed, unarmed, 1, 0, 0, 0);
	}
}

void EffectHealPlayer::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	int maxHealth = PED::GET_PED_MAX_HEALTH(playerPed);

	ENTITY::SET_ENTITY_HEALTH(playerPed, maxHealth, 0);
}

void EffectAlmostDead::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	ENTITY::SET_ENTITY_HEALTH(playerPed, 1, 0);
}

void EffectRestoreStamina::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_ID(), 1.0f);
}

void EffectRagdoll::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	PED::SET_PED_TO_RAGDOLL(playerPed, 3000, 3000, 0, true, true, false);
}

void EffectGiveRandomWeapon::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	int32_t weaponID = rand() % Effect::WeaponHashes.size();

	Hash weaponHash = Effect::WeaponHashes[weaponID];
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectSetDrunk::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	AUDIO::SET_PED_IS_DRUNK(playerPed, true);
	CAM::SHAKE_GAMEPLAY_CAM((char*)"DRUNK_SHAKE", 1.0f);
	PED::SET_PED_CONFIG_FLAG(playerPed, 100, true);
	GRAPHICS::ANIMPOSTFX_PLAY((char*)"PlayerDrunk01");
}

void EffectSetDrunk::OnDeactivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	AUDIO::SET_PED_IS_DRUNK(playerPed, false);
	CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
	PED::SET_PED_CONFIG_FLAG(playerPed, 100, false);
	GRAPHICS::ANIMPOSTFX_STOP((char*)"PlayerDrunk01");
}

void EffectSetDrunk::OnTick()
{
	Effect::OnTick();

	if (GetTickCount() % 4000 == 0)
	{
		PED::SET_PED_TO_RAGDOLL(PLAYER::PLAYER_PED_ID(), 1000, 1000, 0, true, true, false);
	}
}

void EffectClearPursuit::OnActivate()
{
	Player player = PLAYER::PLAYER_ID();
	PURSUIT::CLEAR_CURRENT_PURSUIT();
	PURSUIT::SET_PLAYER_WANTED_INTENSITY(player, 0);
	PURSUIT::SET_PLAYER_PRICE_ON_A_HEAD(player, 0);
}

void EffectIncreaseBounty::OnActivate()
{
	Player player = PLAYER::PLAYER_ID();
	int priceOnHead = PURSUIT::GET_PLAYER_PRICE_ON_A_HEAD(player);
	PURSUIT::SET_PLAYER_PRICE_ON_A_HEAD(player, priceOnHead + 100 * 100);
}

void EffectRemoveCurrentVehicle::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, false);

		VEHICLE::DELETE_VEHICLE(&veh);

		
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		Ped mount = PED::GET_MOUNT(playerPed);

		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);

		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(mount, false, false);

		PED::DELETE_PED(&mount);
	}
}

void EffectGiveLasso::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_LASSO");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectTeleportToVanHorn::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	TeleportPlayerTo(3035.75f, 431.968f, 68.7f);
	ENTITY::SET_ENTITY_HEADING(playerPed, 27);

	ENTITY::SET_ENTITY_VELOCITY(playerPed, 0.0f, 0.0f, 0.0f);
}

void EffectIgnitePlayer::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	FIRE::START_ENTITY_FIRE(playerPed, 1.0f, 0, 0);
}

void EffectKickflip::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

		ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 2.0f, false, false, false, false);

		Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(veh, 0);

		ENTITY::SET_ENTITY_VELOCITY(playerPed, vel.x, vel.y, vel.z);
	}

	PED::SET_PED_TO_RAGDOLL(playerPed, 1000, 1000, 0, true, true, false);

	ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 1, 0, 0, 10, 2, 0, 0, 0, true, true, true, false, true);
}

void SetPlayerModel(const char* model, uint64_t* ptr1_val, uint64_t* ptr2_val)
{
	Hash hash = GAMEPLAY::GET_HASH_KEY((char*)model);

	LoadModel(hash);

	uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;
	*ptr1_val = *ptr1;
	*ptr2_val = *ptr2;

	*ptr1 = *ptr2 = hash;

	WAIT(1000);

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	PED::SET_PED_VISIBLE(playerPed, 1);
}

void ResetPlayerModel(uint64_t ptr1_val, uint64_t ptr2_val)
{
	uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;
	*ptr1 = ptr1_val;
	*ptr2 = ptr2_val;

	//WAIT(1000);

	//Ped playerPed = PLAYER::PLAYER_PED_ID();
	//PED::SET_PED_VISIBLE(playerPed, 1);
}

void EffectLightningWeapons::OnActivate()
{
	lastVec.x = lastVec.y = lastVec.z = 0.0f;
}

void EffectLightningWeapons::OnDeactivate()
{
	lastVec.x = lastVec.y = lastVec.z = 0.0f;
}

void EffectLightningWeapons::OnTick()
{
	Vector3 vec;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);

	if (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z)
	{
		return;
	}

	lastVec = vec;

	/** _FORCE_LIGHTNING_FLASH_AT_COORDS */
	invoke<Void>(0x67943537D179597C, vec.x, vec.y, vec.z);
}

void IEffectSkinChange::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

		ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 2.0f, false, false, false, false);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		Ped mount = PED::GET_MOUNT(playerPed);

		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);
	}

	SetPlayerModel(this->skinToSet, &oldSkin1, &oldSkin2);
}

void IEffectSkinChange::OnDeactivate()
{
	ResetPlayerModel(oldSkin1, oldSkin2);
}

void EffectHonorGood::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	(*getGlobalPtr(0x2BA2)) = 0;

	int honor = *getGlobalPtr(0x2BA2);

	*getGlobalPtr(1347477 + 155 + 1) = 240;
}

void EffectHonorBad::OnActivate() 
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	(*getGlobalPtr(0x2BA2)) = 0;

	int honor = *getGlobalPtr(0x2BA2);

	*getGlobalPtr(1347477 + 155 + 1) = -240;
}

void EffectHonorReset::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	(*getGlobalPtr(0x2BA2)) = 0;

	int honor = *getGlobalPtr(0x2BA2);

	*getGlobalPtr(1347477 + 155 + 1) = 40;
}
