#include "player.h"
#include "peds.h"
#include "../script.h"

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

	invoke<Void>(0x406CCF555B04FAD3, playerPed, true, 1.0f);
}

void EffectSetDrunk::OnDeactivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	AUDIO::SET_PED_IS_DRUNK(playerPed, false);
	CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
	PED::SET_PED_CONFIG_FLAG(playerPed, 100, false);
	GRAPHICS::ANIMPOSTFX_STOP((char*)"PlayerDrunk01");

	invoke<Void>(0x406CCF555B04FAD3, playerPed, false, 0.0f);
}

void EffectSetDrunk::OnTick()
{
	Effect::OnTick();

	if (GetTickCount() % 7000 == 0)
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
	PURSUIT::SET_PLAYER_PRICE_ON_A_HEAD(player, priceOnHead + 50 * 100);
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

	ChaosMod::UpdatePlayerSkinHash();

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
	ChaosMod::ResetPlayerSkin();
	//uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	//uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;
	//*ptr1 = ptr1_val;
	//*ptr2 = ptr2_val;

	//WAIT(1000);

	//Ped playerPed = PLAYER::PLAYER_PED_ID();
	//PED::SET_PED_VISIBLE(playerPed, 1);
}

void EffectLightningWeapons::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &lastVec);
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
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		Ped mount = PED::GET_MOUNT(playerPed);

		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(mount, true, true);

		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);
	}

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

void EffectPlayerSleep::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

		ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 2.0f, false, false, false, false);
	}

	PED::SET_PED_TO_RAGDOLL(playerPed, EffectDuration * 1000 + 3000, EffectDuration * 1000 + 3000, 0, true, true, false);
}

void EffectPlayerIsMinion::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, playerPed, 0.4f);
}

void EffectPlayerIsMinion::OnDeactivate()
{
	Effect::OnDeactivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, playerPed, 1.0f);
}

void EffectTeleportToWaypoint::OnActivate()
{
	Effect::OnDeactivate();

	if (!RADAR::IS_WAYPOINT_ACTIVE())
	{
		return;
	}

	Entity entity = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(entity, true))
	{
		entity = PED::GET_VEHICLE_PED_IS_IN(entity, false);
	}
	else if (PED::IS_PED_ON_MOUNT(entity))
	{
		entity = PED::GET_MOUNT(entity);
	}

	Vector3 coords = RADAR::GET_WAYPOINT_COORDS_3D();

	bool bUpdatedCoords = GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, 100.0, &coords.z, FALSE);

	if (!bUpdatedCoords)
	{
		static const float groundCheckHeight[] = {
				100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
				450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
		};

		for (float height : groundCheckHeight)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords.x, coords.y, height, 0, 0, 1);
			WAIT(100);
			if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, height, &coords.z, FALSE))
			{
				coords.z += 3.0;
				break;
			}
		}
	}

	ENTITY::SET_ENTITY_COORDS(entity, coords.x, coords.y, coords.z, false, false, false, false);
}

void EffectExplosiveWeapons::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &lastVec);
}

void EffectExplosiveWeapons::OnDeactivate()
{
	lastVec.x = lastVec.y = lastVec.z = 0.0f;
}

void EffectExplosiveWeapons::OnTick()
{
	Vector3 vec;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);

	if (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z)
	{
		return;
	}

	lastVec = vec;

	FIRE::ADD_EXPLOSION(vec.x, vec.y, vec.z, 27, 1.0f, true, false, 1.0f);
}

void EffectBloodTrails::OnTick()
{
	if (GetTickCount() % 5000 == 0)
	{
		invoke<Void>(0xC349EE1E6EFA494B, PLAYER::PLAYER_PED_ID(), 1.0f, 1.0f, 1.0f);
	}
}

void EffectSetRandomWalkStyle::OnActivate()
{
	static std::vector<const char*> walkStyles = {
		"cower_known",
		"injured_left_leg"
		"injured_general",
		"injured_right_leg",
		"injured_left_arm",
		"injured_right_arm",
		"injured_torso",
		"very_drunk",
		"moderate_drunk"
	};

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	invoke<Void>(0x923583741DC87BCE, playerPed, "default");
	invoke<Void>(0x89F5E7ADECCCB49C, playerPed, walkStyles[rand() % walkStyles.size()]);
}

void EffectSetRandomWalkStyle::OnDeactivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	invoke<Void>(0x923583741DC87BCE, playerPed, "arthur_healthy");
	invoke<Void>(0x89F5E7ADECCCB49C, playerPed, "default");
}

void EffectTeleportWeapons::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &lastVec);
}

void EffectTeleportWeapons::OnDeactivate()
{
	lastVec.x = lastVec.y = lastVec.z = 0.0f;
}

void EffectTeleportWeapons::OnTick()
{
	Vector3 vec;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);

	if (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z)
	{
		return;
	}

	lastVec = vec;

	ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 0.0f, false, false, false, false);
}

void EffectGiveSniperRifle::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_SNIPERRIFLE_ROLLINGBLOCK");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 35, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 35);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectGiveDynamite::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_THROWN_DYNAMITE");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 3, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 3);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}


void EffectThrowingKnives::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_THROWN_THROWING_KNIVES");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 10, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 10);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectTeleportFewMeters::OnActivate()
{
	Entity entity = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(entity, true))
	{
		entity = PED::GET_VEHICLE_PED_IS_IN(entity, false);
	}
	else if (PED::IS_PED_ON_MOUNT(entity))
	{
		entity = PED::GET_MOUNT(entity);
	}

	Vector3 vec = ENTITY::GET_ENTITY_COORDS(entity, true, 0);

	float radius = 10.0f;

	/** In radians */
	float angle = float(rand() % 360) * (M_PI / 180.0f);

	vec.x += radius * sin(angle);
	vec.y += radius * cos(angle);


	bool bUpdatedCoords = GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vec.x, vec.y, 100.0, &vec.z, FALSE);

	if (!bUpdatedCoords)
	{
		static const float groundCheckHeight[] = {
				100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
				450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
		};

		for (float height : groundCheckHeight)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, vec.x, vec.y, height, 0, 0, 1);
			WAIT(100);
			if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vec.x, vec.y, height, &vec.z, FALSE))
			{
				vec.z += 3.0;
				break;
			}
		}
	}

	ENTITY::SET_ENTITY_COORDS(entity, vec.x, vec.y, vec.z, false, false, false, false);
}

void EffectBlackingOut::OnActivate()
{
	Effect::OnDeactivate();

	GRAPHICS::ANIMPOSTFX_PLAY((char*)"PlayerWakeUpInterrogation");

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

		ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 2.0f, false, false, false, false);
	}

	PED::SET_PED_TO_RAGDOLL(playerPed, 5000, 5000, 0, true, true, false);
}

void EffectRandomClothes::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	int32_t outfitID = rand() % 58;

	if (outfitID == 25 || outfitID == 17 || outfitID == 14 || outfitID == 28)
	{
		outfitID = 0;
	}

	invoke<Void>(0x77FF8D35EEC6BBC4, playerPed, outfitID, false);
}

void EffectMostWanted::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Player player = PLAYER::PLAYER_ID();

	invoke<Void>(0xF60386770878A98F, player, GAMEPLAY::GET_HASH_KEY((char*)"CRIME_ASSAULT_LAW"), 0, 0, 1);
	int priceOnHead = PURSUIT::GET_PLAYER_PRICE_ON_A_HEAD(player);
	PURSUIT::SET_PLAYER_PRICE_ON_A_HEAD(player, priceOnHead + 50 * 100);
}

void EffectPigSkin::OnActivate()
{
	IEffectSkinChange::OnActivate();

	invoke<Void>(0x77FF8D35EEC6BBC4, PLAYER::PLAYER_PED_ID(), rand() % 4, false);
}

void EffectTeleportToLake::OnActivate()
{
	Vector3 vec;
	vec.x = -1.0f;
	vec.y = -1009.0f;
	vec.z = 41.0f;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z, false, false, false, false);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"SKIFF");

	LoadModel(model);

	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, vec.x, vec.y, vec.z, 5.0f, false, false, false, false);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_M_M_UniCorpse_01");

	Ped ped = SpawnPedAroundPlayer(skinModel, false, false);

	MarkPedAsCompanion(ped);

	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 56, false);

	PED::SET_PED_INTO_VEHICLE(ped, veh, -1);
	PED::SET_PED_INTO_VEHICLE(playerPed, veh, 4);

	AI::TASK_VEHICLE_DRIVE_TO_COORD(ped, veh, 40.3f, -701.4f, 40.8f, 1000.0f, 1, model, 16777216, 1.0f, 1.0f);

	/** BF_CanLeaveVehicle */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);

	PED::SET_PED_KEEP_TASK(ped, true);

	static Hash parrotModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Parrot_01");
	Ped parrot = SpawnPedAroundPlayer(parrotModel, false, false);
	MarkPedAsCompanion(parrot);

	ENTITY::SET_ENTITY_COORDS(parrot, vec.x, vec.y, vec.z + 2.0f, false, false, false, false);

	PED::SET_PED_CAN_RAGDOLL(parrot, false);

	AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(parrot, playerPed, 0.0f, 0.0f, 2.0f, 2.5f, -1.0f, -1.0f, 0, 0, 0, 0, 0);

	Vehicle vehCopy = veh;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);
}

void EffectSetWinterOutfit::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Hash playerSkin = ENTITY::GET_ENTITY_MODEL(playerPed);

	static Hash skinArthur = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER_ZERO");
	static Hash skinJohn = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER_THREE");

	if (playerSkin == skinArthur)
	{
		invoke<Void>(0x77FF8D35EEC6BBC4, playerPed, 1, false);
	}
	else if (playerSkin == skinJohn)
	{
		invoke<Void>(0x77FF8D35EEC6BBC4, playerPed, 25, false);
	}
}

void EffectSuperJump::OnTick()
{
	GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
}


void EffectDisableLeftRight::OnTick()
{
	static std::vector<Hash> keys = {
		GET_HASH("INPUT_MOVE_LEFT_ONLY"),			GET_HASH("INPUT_MOVE_RIGHT_ONLY"),
		GET_HASH("INPUT_MOVE_LR"),					GET_HASH("INPUT_VEH_MOVE_LR"),
		GET_HASH("INPUT_MOVE_LEFT"),				GET_HASH("INPUT_MOVE_RIGHT"),
		GET_HASH("INPUT_HORSE_MOVE_LEFT_ONLY"),		GET_HASH("INPUT_VEH_CAR_TURN_LEFT_ONLY"),
		GET_HASH("INPUT_VEH_DRAFT_TURN_LEFT_ONLY"), GET_HASH("INPUT_VEH_BOAT_TURN_LEFT_ONLY"),
		GET_HASH("INPUT_VEH_MOVE_LEFT_ONLY"),		GET_HASH("INPUT_FRONTEND_AXIS_X"),
		GET_HASH("INPUT_HORSE_MOVE_LR"),			GET_HASH("INPUT_HORSE_MOVE_RIGHT_ONLY"),
		GET_HASH("INPUT_VEH_CAR_TURN_LR"),			GET_HASH("INPUT_VEH_DRAFT_TURN_RIGHT_ONLY"),
		GET_HASH("INPUT_VEH_CAR_TURN_RIGHT_ONLY"),	GET_HASH("INPUT_VEH_MOVE_RIGHT_ONLY"),
		GET_HASH("INPUT_VEH_DRAFT_TURN_LR"),		GET_HASH("INPUT_VEH_BOAT_TURN_LR"),
		GET_HASH("INPUT_VEH_BOAT_TURN_RIGHT_ONLY"), GET_HASH("INPUT_FRONTEND_NAV_RIGHT")

	};

	for (Hash key : keys)
	{
		CONTROLS::DISABLE_CONTROL_ACTION(0, key, true);
	}
}

void EffectDisableForwardBackward::OnTick()
{
	static std::vector<Hash> keys = {
		GET_HASH("INPUT_MOVE_UP_ONLY"),				GET_HASH("INPUT_MOVE_DOWN_ONLY"),
		GET_HASH("INPUT_VEH_ACCELERATE"),			GET_HASH("INPUT_VEH_BRAKE"),
		GET_HASH("INPUT_MOVE_UP"),					GET_HASH("INPUT_MOVE_DOWN"),
		GET_HASH("INPUT_MOVE_UD"),					GET_HASH("INPUT_FRONTEND_AXIS_Y"),
		GET_HASH("INPUT_HORSE_MOVE_DOWN_ONLY"),		GET_HASH("INPUT_HORSE_MOVE_UD"),
		GET_HASH("INPUT_VEH_MOVE_DOWN_ONLY"),		GET_HASH("INPUT_VEH_DRAFT_MOVE_UD"),
		GET_HASH("INPUT_HORSE_MOVE_UP_ONLY"),		GET_HASH("INPUT_VEH_MOVE_UP_ONLY"),
		GET_HASH("INPUT_VEH_MOVE_UD"),				GET_HASH("INPUT_FRONTEND_NAV_UP"),
		GET_HASH("INPUT_HORSE_SPRINT"),				GET_HASH("INPUT_VEH_ACCELERATE"),
		GET_HASH("INPUT_HORSE_STOP"),				GET_HASH("INPUT_VEH_HANDCART_BRAKE"),
		GET_HASH("INPUT_VEH_BOAT_BRAKE"),			GET_HASH("INPUT_VEH_HANDBRAKE"),
		GET_HASH("INPUT_VEH_BRAKE"),				GET_HASH("INPUT_VEH_CAR_BRAKE")
	};

	for (Hash key : keys)
	{
		CONTROLS::DISABLE_CONTROL_ACTION(0, key, true);
	}
}

void EffectDisableSprintJump::OnTick()
{
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_SPRINT"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_JUMP"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_HORSE_SPRINT"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_VEH_ACCELERATE"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_HORSE_JUMP"), true);
}

void EffectDisableAllMovements::OnTick()
{
	static std::vector<Hash> keys = {
		GET_HASH("INPUT_MOVE_LEFT_ONLY"),			GET_HASH("INPUT_MOVE_RIGHT_ONLY"),
		GET_HASH("INPUT_MOVE_LR"),					GET_HASH("INPUT_VEH_MOVE_LR"),
		GET_HASH("INPUT_MOVE_LEFT"),				GET_HASH("INPUT_MOVE_RIGHT"),
		GET_HASH("INPUT_HORSE_MOVE_LEFT_ONLY"),		GET_HASH("INPUT_VEH_CAR_TURN_LEFT_ONLY"),
		GET_HASH("INPUT_VEH_DRAFT_TURN_LEFT_ONLY"), GET_HASH("INPUT_VEH_BOAT_TURN_LEFT_ONLY"),
		GET_HASH("INPUT_VEH_MOVE_LEFT_ONLY"),		GET_HASH("INPUT_FRONTEND_AXIS_X"),
		GET_HASH("INPUT_HORSE_MOVE_LR"),			GET_HASH("INPUT_HORSE_MOVE_RIGHT_ONLY"),
		GET_HASH("INPUT_VEH_CAR_TURN_LR"),			GET_HASH("INPUT_VEH_DRAFT_TURN_RIGHT_ONLY"),
		GET_HASH("INPUT_VEH_CAR_TURN_RIGHT_ONLY"),	GET_HASH("INPUT_VEH_MOVE_RIGHT_ONLY"),
		GET_HASH("INPUT_VEH_DRAFT_TURN_LR"),		GET_HASH("INPUT_VEH_BOAT_TURN_LR"),
		GET_HASH("INPUT_VEH_BOAT_TURN_RIGHT_ONLY"), GET_HASH("INPUT_FRONTEND_NAV_RIGHT"),

		GET_HASH("INPUT_MOVE_UP_ONLY"),				GET_HASH("INPUT_MOVE_DOWN_ONLY"),
		GET_HASH("INPUT_VEH_ACCELERATE"),			GET_HASH("INPUT_VEH_BRAKE"),
		GET_HASH("INPUT_MOVE_UP"),					GET_HASH("INPUT_MOVE_DOWN"),
		GET_HASH("INPUT_MOVE_UD"),					GET_HASH("INPUT_FRONTEND_AXIS_Y"),
		GET_HASH("INPUT_HORSE_MOVE_DOWN_ONLY"),		GET_HASH("INPUT_HORSE_MOVE_UD"),
		GET_HASH("INPUT_VEH_MOVE_DOWN_ONLY"),		GET_HASH("INPUT_VEH_DRAFT_MOVE_UD"),
		GET_HASH("INPUT_HORSE_MOVE_UP_ONLY"),		GET_HASH("INPUT_VEH_MOVE_UP_ONLY"),
		GET_HASH("INPUT_VEH_MOVE_UD"),				GET_HASH("INPUT_FRONTEND_NAV_UP"),
		GET_HASH("INPUT_HORSE_SPRINT"),				GET_HASH("INPUT_VEH_ACCELERATE"),
		GET_HASH("INPUT_HORSE_STOP"),				GET_HASH("INPUT_VEH_HANDCART_BRAKE"),
		GET_HASH("INPUT_VEH_BOAT_BRAKE"),			GET_HASH("INPUT_VEH_HANDBRAKE"),
		GET_HASH("INPUT_VEH_BRAKE"),				GET_HASH("INPUT_VEH_CAR_BRAKE")

	};


	for (Hash key : keys)
	{
		CONTROLS::DISABLE_CONTROL_ACTION(0, key, true);
	}
}

void EffectDisableAttackButton::OnTick()
{
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_ATTACK"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_MELEE_ATTACK"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_MELEE_GRAPPLE_ATTACK"), true);
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_HORSE_MELEE"), true);
}

void EffectDisableAiming::OnActivate()
{
	/** Should remove reticle */
	UI::_0x4CC5F2FC1332577F(GET_HASH("HUD_CTX_IN_FAST_TRAVEL_MENU"));
}

void EffectDisableAiming::OnDeactivate()
{
	UI::_0x8BC7C1F929D07BF3(GET_HASH("HUD_CTX_IN_FAST_TRAVEL_MENU"));
}

void EffectDisableAiming::OnTick()
{
	CONTROLS::DISABLE_CONTROL_ACTION(0, GAMEPLAY::GET_HASH_KEY((char*)"INPUT_AIM"), true);


}
