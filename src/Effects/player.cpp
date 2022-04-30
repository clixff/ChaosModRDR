#include "player.h"
#include "peds.h"
#include "../script.h"
#include "misc.h"
#include "vehs.h"

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
		FixEntityInCutscene(entityToUse);
		WAIT(75);
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

void DisableAllMovements()
{
	static std::vector <Hash> keys = {GET_HASH("INPUT_MOVE_LEFT_ONLY"), GET_HASH("INPUT_MOVE_RIGHT_ONLY"),
									  GET_HASH("INPUT_MOVE_LR"), GET_HASH("INPUT_VEH_MOVE_LR"),
									  GET_HASH("INPUT_MOVE_LEFT"), GET_HASH("INPUT_MOVE_RIGHT"),
									  GET_HASH("INPUT_HORSE_MOVE_LEFT_ONLY"), GET_HASH("INPUT_VEH_CAR_TURN_LEFT_ONLY"),
									  GET_HASH("INPUT_VEH_DRAFT_TURN_LEFT_ONLY"),
									  GET_HASH("INPUT_VEH_BOAT_TURN_LEFT_ONLY"), GET_HASH("INPUT_VEH_MOVE_LEFT_ONLY"),
									  GET_HASH("INPUT_FRONTEND_AXIS_X"), GET_HASH("INPUT_HORSE_MOVE_LR"),
									  GET_HASH("INPUT_HORSE_MOVE_RIGHT_ONLY"), GET_HASH("INPUT_VEH_CAR_TURN_LR"),
									  GET_HASH("INPUT_VEH_DRAFT_TURN_RIGHT_ONLY"),
									  GET_HASH("INPUT_VEH_CAR_TURN_RIGHT_ONLY"), GET_HASH("INPUT_VEH_MOVE_RIGHT_ONLY"),
									  GET_HASH("INPUT_VEH_DRAFT_TURN_LR"), GET_HASH("INPUT_VEH_BOAT_TURN_LR"),
									  GET_HASH("INPUT_VEH_BOAT_TURN_RIGHT_ONLY"), GET_HASH("INPUT_FRONTEND_NAV_RIGHT"),
	
									  GET_HASH("INPUT_MOVE_UP_ONLY"), GET_HASH("INPUT_MOVE_DOWN_ONLY"),
									  GET_HASH("INPUT_VEH_ACCELERATE"), GET_HASH("INPUT_VEH_BRAKE"),
									  GET_HASH("INPUT_MOVE_UP"), GET_HASH("INPUT_MOVE_DOWN"), GET_HASH("INPUT_MOVE_UD"),
									  GET_HASH("INPUT_FRONTEND_AXIS_Y"), GET_HASH("INPUT_HORSE_MOVE_DOWN_ONLY"),
									  GET_HASH("INPUT_HORSE_MOVE_UD"), GET_HASH("INPUT_VEH_MOVE_DOWN_ONLY"),
									  GET_HASH("INPUT_VEH_DRAFT_MOVE_UD"), GET_HASH("INPUT_HORSE_MOVE_UP_ONLY"),
									  GET_HASH("INPUT_VEH_MOVE_UP_ONLY"), GET_HASH("INPUT_VEH_MOVE_UD"),
									  GET_HASH("INPUT_FRONTEND_NAV_UP"), GET_HASH("INPUT_HORSE_SPRINT"),
									  GET_HASH("INPUT_VEH_ACCELERATE"), GET_HASH("INPUT_HORSE_STOP"),
									  GET_HASH("INPUT_VEH_HANDCART_BRAKE"), GET_HASH("INPUT_VEH_BOAT_BRAKE"),
									  GET_HASH("INPUT_VEH_HANDBRAKE"), GET_HASH("INPUT_VEH_BRAKE"),
									  GET_HASH("INPUT_VEH_CAR_BRAKE")};
	
	
	for (Hash key : keys)
	{
		CONTROLS::DISABLE_CONTROL_ACTION(0, key, true);
	}
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
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_RIFLE_SPRINGFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectGiveRevolver::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_REVOLVER_SCHOFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectNoAmmo::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** REMOVE_ALL_PED_AMMO */
	invoke<Void>(0x1B83C0DEEBCBB214, playerPed);
	
	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_UNARMED");
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
	
	static std::set <Hash> importantWeapons = {GET_HASH("WEAPON_KIT_CAMERA"), GET_HASH("WEAPON_LASSO"),
											   GET_HASH("WEAPON_FISHINGROD"), GET_HASH("WEAPON_MELEE_LANTERN_ELECTRIC"),
											   GET_HASH("WEAPON_MELEE_KNIFE")};
	
	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_UNARMED");
	Hash weaponHash = 0;
	
	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, 0, 0, 0) && WEAPON::IS_WEAPON_VALID(weaponHash) &&
		weaponHash != unarmed)
	{
		if (importantWeapons.contains(weaponHash))
		{
			return;
		}
		
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
	FixEntityInCutscene(playerPed);
	WAIT(75);
	PED::SET_PED_TO_RAGDOLL(playerPed, 3000, 3000, 0, true, true, false);
}

void EffectGiveRandomWeapon::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	static std::set <Hash> importantWeapons = {GET_HASH("WEAPON_KIT_CAMERA"), GET_HASH("WEAPON_LASSO"),
											   GET_HASH("WEAPON_FISHINGROD"), GET_HASH("WEAPON_MELEE_LANTERN_ELECTRIC"),
											   GET_HASH("WEAPON_MELEE_KNIFE")};
	
	for (auto wHash : importantWeapons)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, wHash, 100, 1, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(playerPed, wHash, 100);
	}
	
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
	CAM::SHAKE_GAMEPLAY_CAM((char*) "DRUNK_SHAKE", 1.0f);
	PED::SET_PED_CONFIG_FLAG(playerPed, 100, true);
	GRAPHICS::ANIMPOSTFX_PLAY((char*) "PlayerDrunk01");
	
	invoke<Void>(0x406CCF555B04FAD3, playerPed, true, 1.0f);
	
	/** Set walking style */
	invoke<Void>(0x89F5E7ADECCCB49C, playerPed, "very_drunk");
}

void EffectSetDrunk::OnDeactivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	AUDIO::SET_PED_IS_DRUNK(playerPed, false);
	CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
	PED::SET_PED_CONFIG_FLAG(playerPed, 100, false);
	GRAPHICS::ANIMPOSTFX_STOP((char*) "PlayerDrunk01");
	
	invoke<Void>(0x406CCF555B04FAD3, playerPed, false, 0.0f);
	invoke<Void>(0x923583741DC87BCE, playerPed, "arthur_healthy");
	invoke<Void>(0x89F5E7ADECCCB49C, playerPed, "default");
}

void EffectSetDrunk::OnTick()
{
	Effect::OnTick();
	
	if (TimerTick(7000))
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
		
		if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(mount))
		{
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(mount, false, false);
			
			PED::DELETE_PED(&mount);
		}
	}
}

void EffectGiveLasso::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_LASSO");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
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
	
	FixEntityInCutscene(playerPed);
	WAIT(75);
	
	PED::SET_PED_TO_RAGDOLL(playerPed, 1000, 1000, 0, true, true, false);
	
	ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 1, 0, 0, 10, 2, 0, 0, 0, true, true, true, false, true);
}

void SetPlayerModel(const char* model, uint64_t* ptr1_val, uint64_t* ptr2_val)
{
	Hash hash = GET_HASH(model);
	
	LoadModel(hash);
	
	ChaosMod::Singleton->SavePlayerAttributes();
	
	ChaosMod::UpdatePlayerSkinHash();
	
	PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), hash, 1);
	*getGlobalPtr(0x23) = PLAYER::PLAYER_PED_ID();
	
	
	uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;
	
	*ptr1 = hash;
	*ptr2 = hash;
	Ped playerPed = PLAYER::PLAYER_PED_ID();;
	*getGlobalPtr(0x23) = playerPed;
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
	ENTITY::SET_ENTITY_COLLISION(playerPed, true, true);
	ENTITY::SET_ENTITY_DYNAMIC(playerPed, true);
}

void ResetPlayerModel(uint64_t ptr1_val, uint64_t ptr2_val)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);
	}
	ChaosMod::ResetPlayerSkin();
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
	
	bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);
	
	if (!bCoordValid || (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z))
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
		
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(mount, true, true);
		
		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, playerPed, 1, 0);
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
	
	FixEntityInCutscene(playerPed);
	
	WAIT(75);
	
	PED::SET_PED_TO_RAGDOLL(playerPed, EffectDuration * 1000 + 3000, EffectDuration * 1000 + 3000, 0, true, true,
							false);
}

void EffectPlayerIsMinion::OnActivate()
{
	Effect::OnActivate();
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, playerPed, 0.4f);
}

void EffectPlayerIsMinion::OnTick()
{
	if (TimerTick(500))
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		
		/** _SET_PED_SCALE */
		invoke<Void>(0x25ACFC650B65C538, playerPed, 0.4f);
	}
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
	
	Entity entity = PLAYER::PLAYER_PED_ID();
	
	
	if (!RADAR::IS_WAYPOINT_ACTIVE())
	{
		return;
	}
	
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
		static const float groundCheckHeight[] = {100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0,
												  500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0};
		
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
	
	bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);
	
	if (!bCoordValid || (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z))
	{
		return;
	}
	
	lastVec = vec;
	
	FIRE::ADD_EXPLOSION(vec.x, vec.y, vec.z, 27, 1.0f, true, false, 1.0f);
}

void EffectBloodTrails::OnTick()
{
	if (TimerTick(5000))
	{
		invoke<Void>(0xC349EE1E6EFA494B, PLAYER::PLAYER_PED_ID(), 1.0f, 1.0f, 1.0f);
	}
}

void EffectSetRandomWalkStyle::OnActivate()
{
	static std::vector<const char*> walkStyles = {"cower_known", "injured_left_leg"
																 "injured_general", "injured_right_leg",
												  "injured_left_arm", "injured_right_arm", "injured_torso",
												  "very_drunk", "moderate_drunk"};
	
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
	
	bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);
	
	if (!bCoordValid || (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z))
	{
		return;
	}
	
	lastVec = vec;
	
	ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 0.0f, false, false, false, false);
}

void EffectGiveSniperRifle::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_SNIPERRIFLE_ROLLINGBLOCK");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 35, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 35);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}

void EffectGiveDynamite::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_THROWN_DYNAMITE");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 3, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 3);
	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
}


void EffectThrowingKnives::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_THROWN_THROWING_KNIVES");
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
	
	Vector3 vec = GetRandomCoordInRange(ENTITY::GET_ENTITY_COORDS(entity, true, 0), 10);
	
	bool bUpdatedCoords = GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vec.x, vec.y, vec.z, &vec.z, FALSE);
	
	if (!bUpdatedCoords)
	{
		static const float groundCheckHeight[] = {100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0,
												  500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0};
		
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
	
	GRAPHICS::ANIMPOSTFX_PLAY((char*) "PlayerWakeUpInterrogation");
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
		
		ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 2.0f, false, false, false, false);
	}
	
	FixEntityInCutscene(playerPed);
	WAIT(75);
	
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
	
	invoke<Void>(0xF60386770878A98F, player, GAMEPLAY::GET_HASH_KEY((char*) "CRIME_ASSAULT_LAW"), 0, 0, 1);
	int priceOnHead = PURSUIT::GET_PLAYER_PRICE_ON_A_HEAD(player);
	PURSUIT::SET_PLAYER_PRICE_ON_A_HEAD(player, priceOnHead + 50 * 100);
}

void EffectPigSkin::OnActivate()
{
	IEffectSkinChange::OnActivate();
	
	invoke<Void>(0x77FF8D35EEC6BBC4, PLAYER::PLAYER_PED_ID(), rand() % 4, false);
}

void EffectSetWinterOutfit::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	Hash playerSkin = ENTITY::GET_ENTITY_MODEL(playerPed);
	
	static Hash skinArthur = GAMEPLAY::GET_HASH_KEY((char*) "PLAYER_ZERO");
	static Hash skinJohn = GAMEPLAY::GET_HASH_KEY((char*) "PLAYER_THREE");
	
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
	static std::vector <Hash> keys = {GET_HASH("INPUT_MOVE_LEFT_ONLY"), GET_HASH("INPUT_MOVE_RIGHT_ONLY"),
									  GET_HASH("INPUT_MOVE_LR"), GET_HASH("INPUT_VEH_MOVE_LR"),
									  GET_HASH("INPUT_MOVE_LEFT"), GET_HASH("INPUT_MOVE_RIGHT"),
									  GET_HASH("INPUT_HORSE_MOVE_LEFT_ONLY"), GET_HASH("INPUT_VEH_CAR_TURN_LEFT_ONLY"),
									  GET_HASH("INPUT_VEH_DRAFT_TURN_LEFT_ONLY"),
									  GET_HASH("INPUT_VEH_BOAT_TURN_LEFT_ONLY"), GET_HASH("INPUT_VEH_MOVE_LEFT_ONLY"),
									  GET_HASH("INPUT_FRONTEND_AXIS_X"), GET_HASH("INPUT_HORSE_MOVE_LR"),
									  GET_HASH("INPUT_HORSE_MOVE_RIGHT_ONLY"), GET_HASH("INPUT_VEH_CAR_TURN_LR"),
									  GET_HASH("INPUT_VEH_DRAFT_TURN_RIGHT_ONLY"),
									  GET_HASH("INPUT_VEH_CAR_TURN_RIGHT_ONLY"), GET_HASH("INPUT_VEH_MOVE_RIGHT_ONLY"),
									  GET_HASH("INPUT_VEH_DRAFT_TURN_LR"), GET_HASH("INPUT_VEH_BOAT_TURN_LR"),
									  GET_HASH("INPUT_VEH_BOAT_TURN_RIGHT_ONLY"), GET_HASH("INPUT_FRONTEND_NAV_RIGHT")
		
	};
	
	for (Hash key : keys)
	{
		CONTROLS::DISABLE_CONTROL_ACTION(0, key, true);
	}
}

void EffectDisableForwardBackward::OnTick()
{
	static std::vector <Hash> keys = {GET_HASH("INPUT_MOVE_UP_ONLY"), GET_HASH("INPUT_MOVE_DOWN_ONLY"),
									  GET_HASH("INPUT_VEH_ACCELERATE"), GET_HASH("INPUT_VEH_BRAKE"),
									  GET_HASH("INPUT_MOVE_UP"), GET_HASH("INPUT_MOVE_DOWN"), GET_HASH("INPUT_MOVE_UD"),
									  GET_HASH("INPUT_FRONTEND_AXIS_Y"), GET_HASH("INPUT_HORSE_MOVE_DOWN_ONLY"),
									  GET_HASH("INPUT_HORSE_MOVE_UD"), GET_HASH("INPUT_VEH_MOVE_DOWN_ONLY"),
									  GET_HASH("INPUT_VEH_DRAFT_MOVE_UD"), GET_HASH("INPUT_HORSE_MOVE_UP_ONLY"),
									  GET_HASH("INPUT_VEH_MOVE_UP_ONLY"), GET_HASH("INPUT_VEH_MOVE_UD"),
									  GET_HASH("INPUT_FRONTEND_NAV_UP"), GET_HASH("INPUT_HORSE_SPRINT"),
									  GET_HASH("INPUT_VEH_ACCELERATE"), GET_HASH("INPUT_HORSE_STOP"),
									  GET_HASH("INPUT_VEH_HANDCART_BRAKE"), GET_HASH("INPUT_VEH_BOAT_BRAKE"),
									  GET_HASH("INPUT_VEH_HANDBRAKE"), GET_HASH("INPUT_VEH_BRAKE"),
									  GET_HASH("INPUT_VEH_CAR_BRAKE")};
	
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
	DisableAllMovements();
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
	CONTROLS::DISABLE_CONTROL_ACTION(0, GAMEPLAY::GET_HASH_KEY((char*) "INPUT_AIM"), true);
}

void EffectRandomHonor::OnActivate()
{
	*getGlobalPtr(1347477 + 155 + 1) = (rand() % 51) - 25;
}

void EffectInvertVelocity::OnActivate()
{
	Entity entity = PLAYER::PLAYER_PED_ID();
	
	bool bUsingVehicle = PED::IS_PED_IN_ANY_VEHICLE(entity, true);
	
	if (bUsingVehicle)
	{
		entity = PED::GET_VEHICLE_PED_IS_IN(entity, false);
	}
	else if (PED::IS_PED_ON_MOUNT(entity))
	{
		entity = PED::GET_MOUNT(entity);
	}
	
	float multiplier = -3.0f;
	
	Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(entity, 0);
	
	if (!bUsingVehicle)
	{
		FixEntityInCutscene(entity);
		WAIT(75);
		PED::SET_PED_TO_RAGDOLL(entity, 1000, 1000, 0, true, true, false);
	}
	else
	{
		multiplier = -5.0f;
	}
	
	vel.x *= multiplier;
	vel.y *= multiplier;
	vel.z *= multiplier;
	
	
	ENTITY::SET_ENTITY_VELOCITY(entity, vel.x, vel.y, vel.z);
}

void EffectIncreaseVelocity::OnActivate()
{
	Entity entity = PLAYER::PLAYER_PED_ID();
	
	bool bUsingVehicle = PED::IS_PED_IN_ANY_VEHICLE(entity, true);
	
	if (bUsingVehicle)
	{
		entity = PED::GET_VEHICLE_PED_IS_IN(entity, false);
	}
	else if (PED::IS_PED_ON_MOUNT(entity))
	{
		entity = PED::GET_MOUNT(entity);
	}
	
	float multiplier = 3.0f;
	Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(entity, 0);
	
	if (!bUsingVehicle)
	{
		FixEntityInCutscene(entity);
		WAIT(75);
		PED::SET_PED_TO_RAGDOLL(entity, 1000, 1000, 0, true, true, false);
	}
	
	
	vel.x *= multiplier;
	vel.y *= multiplier;
	vel.z *= multiplier;
	
	ENTITY::SET_ENTITY_VELOCITY(entity, vel.x, vel.y, vel.z);
}

void EffectBunnyhop::OnTick()
{
	if (!TimerTick(500))
	{
		return;
	}
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
		
		ENTITY::SET_ENTITY_COORDS(playerPed, vec.x, vec.y, vec.z + 1.5f, false, false, false, false);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);
	}
	else
	{
		AI::TASK_JUMP(playerPed, 0);
	}
}

void EffectEyeDisorder::OnActivate()
{
	GRAPHICS::ANIMPOSTFX_PLAY((char*) "OJDominoBlur");
}

void EffectEyeDisorder::OnDeactivate()
{
	GRAPHICS::ANIMPOSTFX_STOP((char*) "OJDominoBlur");
}


void EffectBirdSkin::OnActivate()
{
	static std::vector<const char*> birds = {"A_C_BlueJay_01", "A_C_Cardinal_01", "A_C_CarolinaParakeet_01",
											 "A_C_CedarWaxwing_01", "A_C_Chicken_01", "A_C_Cormorant_01",
											 "A_C_CraneWhooping_01", "A_C_Crow_01", "A_C_Duck_01", "A_C_Eagle_01",
											 "A_C_Egret_01", "A_C_Hawk_01", "A_C_Heron_01", "A_C_Loon_01", "A_C_Owl_01",
											 "A_C_Parrot_01", "A_C_Pelican_01", "A_C_Pheasant_01", "A_C_Pigeon",
											 "A_C_PrairieChicken_01", "A_C_Quail_01", "A_C_Raven_01",
											 "A_C_RedFootedBooby_01", "A_C_Rooster_01", "A_C_RoseateSpoonbill_01",
											 "A_C_Seagull_01", "A_C_TurkeyWild_01", "A_C_Vulture_01",
											 "A_C_Woodpecker_01", "A_C_Woodpecker_02"};
	
	auto modelName = birds[rand() % birds.size()];
	this->skinToSet = modelName;
	
	IEffectSkinChange::OnActivate();
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	uint32_t maxOutfits = PED::_0x10C70A515BC03707(playerPed);
	
	uint32_t randOutfit = rand() % maxOutfits;
	
	invoke<Void>(0x77FF8D35EEC6BBC4, playerPed, randOutfit, false);
}

void EffectBirdSkin::OnTick()
{
	static Hash controlSet = GET_HASH("OnMount");
	CONTROLS::_0x2804658EB7D8A50B(2, controlSet);
}

void EffectBodySwap::OnActivate()
{
	pedSkin = 0;
	clone = 0;
	auto nearbyPeds = GetNearbyPeds(50);
	
	std::vector <Ped> validPeds;
	std::vector <Ped> missionPeds;
	
	
	bool bUseMissionPed = false;
	bool bUseRandomSkin = false;
	
	for (auto ped : nearbyPeds)
	{
		if (PED::IS_PED_HUMAN(ped))
		{
			if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped) || ChaosMod::pedsSet.contains(ped))
			{
				validPeds.push_back(ped);
			}
			else
			{
				missionPeds.push_back(ped);
			}
		}
	}
	
	if (!validPeds.size())
	{
		if (missionPeds.size())
		{
			bUseMissionPed = true;
		}
		else
		{
			bUseRandomSkin = true;
		}
	}
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	Ped ped = 0;
	
	if (!bUseRandomSkin)
	{
		if (bUseMissionPed)
		{
			ped = missionPeds[rand() % missionPeds.size()];
		}
		else
		{
			ped = validPeds[rand() % validPeds.size()];
		}
		
		if (!ENTITY::DOES_ENTITY_EXIST(ped))
		{
			return;
		}
	}
	
	
	Vector3 playerVec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	Vector3 pedVec;
	pedVec.x = pedVec.y = pedVec.z = 0;
	
	auto pedHeading = 0;
	
	Vehicle playerVehicle = 0;
	Ped playerMount = 0;
	uint32_t playerSeat = -2;
	
	Vehicle pedVehicle = 0;
	Ped pedMount = 0;
	uint32_t pedSeat = -2;
	
	if (!bUseRandomSkin)
	{
		pedSkin = ENTITY::GET_ENTITY_MODEL(ped);
	}
	else
	{
		static std::vector<const char*> skins = {"a_m_m_vallaborer_01", "a_m_m_valtownfolk_01", "a_m_m_valtownfolk_02",
												 "a_m_m_tumtownfolk_01", "a_f_m_valtownfolk_01",
												 "a_m_m_asbtownfolk_01"};
		
		pedSkin = GET_HASH(skins[rand() % skins.size()]);
	}
	
	
	if (!bUseMissionPed && !bUseRandomSkin)
	{
		pedVec = ENTITY::GET_ENTITY_COORDS(ped, true, 0);
		pedHeading = ENTITY::GET_ENTITY_HEADING(ped);
		ENTITY::SET_ENTITY_COORDS(playerPed, pedVec.x, pedVec.y, pedVec.z, false, false, false, false);
		
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
		{
			playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
			
			playerSeat = PED::_0x4E76CB57222A00E5(playerPed);
			
			ENTITY::SET_ENTITY_COORDS(playerPed, playerVec.x, playerVec.y, playerVec.z, false, false, false, false);
		}
		else if (PED::IS_PED_ON_MOUNT(playerPed))
		{
			playerMount = PED::GET_MOUNT(playerPed);
			
			playerSeat = PED::_0x4E76CB57222A00E5(playerPed);
			
			/** _REMOVE_PED_FROM_MOUNT */
			invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);
			
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(playerMount, true, true);
		}
		
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, true))
		{
			pedVehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
			
			pedSeat = PED::_0x4E76CB57222A00E5(ped);
		}
		else if (PED::IS_PED_ON_MOUNT(ped))
		{
			pedMount = PED::GET_MOUNT(ped);
			
			pedSeat = PED::_0x4E76CB57222A00E5(ped);
		}
		
		PED::DELETE_PED(&ped);
		
		clone = PED::CLONE_PED(playerPed, ENTITY::GET_ENTITY_HEADING(playerPed), true, false);
		
		if (playerVehicle)
		{
			PED::SET_PED_INTO_VEHICLE(clone, playerVehicle, playerSeat);
			AI::TASK_VEHICLE_DRIVE_WANDER(clone, playerVehicle, 100000.0f, 0x400c0025);
			
		}
		else if (playerMount)
		{
			invoke<Void>(0x028F76B6E78246EB, clone, playerMount, playerSeat, true);
			AI::TASK_WANDER_STANDARD(clone, 100.0f, 10);
			
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS(clone, playerVec.x, playerVec.y, playerVec.z, false, false, false, false);
			AI::TASK_WANDER_STANDARD(clone, 100.0f, 10);
		}
		
		if (ENTITY::DOES_ENTITY_EXIST(clone))
		{
			ChaosMod::pedsSet.insert(clone);
		}
	}
	
	LoadModel(pedSkin);
	
	ChaosMod::Singleton->SavePlayerAttributes();
	
	ChaosMod::UpdatePlayerSkinHash();
	
	PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), pedSkin, 1);
	
	*getGlobalPtr(0x23) = PLAYER::PLAYER_PED_ID();
	
	uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;
	
	*ptr1 = *ptr2 = pedSkin;
	
	*getGlobalPtr(0x23) = PLAYER::PLAYER_PED_ID();
	
	playerPed = PLAYER::PLAYER_PED_ID();
	
	ENTITY::SET_ENTITY_COLLISION(playerPed, true, true);
	ENTITY::SET_ENTITY_DYNAMIC(playerPed, true);
	
	if (!bUseMissionPed && !bUseRandomSkin)
	{
		if (pedVehicle)
		{
			PED::SET_PED_INTO_VEHICLE(playerPed, pedVehicle, pedSeat);
		}
		else if (pedMount)
		{
			invoke<Void>(0x028F76B6E78246EB, playerPed, pedMount, pedSeat, true);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS(playerPed, pedVec.x, pedVec.y, pedVec.z, false, false, false, false);
			ENTITY::SET_ENTITY_HEADING(playerPed, pedHeading);
		}
	}
	
	if (bUseRandomSkin)
	{
		uint32_t maxOutfits = PED::_0x10C70A515BC03707(playerPed);
		uint32_t randOutfit = rand() % maxOutfits;
	}
}

void EffectBodySwap::OnDeactivate()
{
	if (ENTITY::DOES_ENTITY_EXIST(clone))
	{
		Ped ped = SpawnPedAroundPlayer(pedSkin, false, false);
		
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(clone, true, 0);
		
		ENTITY::SET_ENTITY_COORDS(ped, vec.x, vec.y, vec.z, false, false, false, false);
		
		AI::TASK_WANDER_STANDARD(ped, 100.0f, 10);
		
		ChaosMod::pedsSet.erase(clone);
		
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(clone, false, false);
		
		PED::DELETE_PED(&clone);
	}
	
	ResetPlayerModel(0, 0);
	
	pedSkin = 0;
	clone = 0;
}

void EffectPlayerSpin::OnActivate()
{
	heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
}

void EffectPlayerSpin::OnTick()
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
	
	heading += 625.0f * ChaosMod::GetDeltaTimeSeconds();
	
	heading = fmod(heading, 360.0f);
	
	ENTITY::SET_ENTITY_HEADING(entity, heading);
}

void EffectPlayerLosesWeight::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	PED::_0x1902C4CFCC5BE57C(playerPed, 0x63F130D5);
	PED::_0x1902C4CFCC5BE57C(playerPed, 0x86155956);
	PED::_0x1902C4CFCC5BE57C(playerPed, 0x652668B6);
	PED::_0xCC8CA3E88256E58F(playerPed, 0, 1, 1, 1, false);
}

void EffectPlayerGainsWeight::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	PED::_0x1902C4CFCC5BE57C(playerPed, 0x63F130D5);
	PED::_0x1902C4CFCC5BE57C(playerPed, 0x74D74B1C);
	PED::_0x1902C4CFCC5BE57C(playerPed, 0xBB7091D9);
	PED::_0xCC8CA3E88256E58F(playerPed, 0, 1, 1, 1, false);
}

void EffectSetRandomHat::OnActivate()
{
	static std::vector <Hash> hats = {0x2514B2B9, 0x05A94693, 0xB2A7CB98, 0x2968E73D, 0xAE8ACE4E, 0xD16013FC,
									  0x3D9CEC78, 0x5F74300A, 0x48760F4A};
	
	Hash hat = hats[rand() % hats.size()];
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	PED::_0x1902C4CFCC5BE57C(playerPed, hat);
	PED::_0xCC8CA3E88256E58F(playerPed, 0, 1, 1, 1, false);
}

void EffectGravityGun::OnActivate()
{
	lastVec.x = lastVec.y = lastVec.z = 0.0f;
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_UNARMED");
	Hash weaponHash = 0;
	
	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, 0, 0, 0) && weaponHash == unarmed)
	{
		static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_PISTOL_MAUSER_DRUNK");
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, weaponHash, 35, 1, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(playerPed, weaponHash, 35);
		WEAPON::SET_CURRENT_PED_WEAPON(playerPed, weaponHash, 1, 0, 0, 0);
	}
}

void EffectGravityGun::OnDeactivate()
{
	lastVec.x = lastVec.y = lastVec.z = 0.0f;
}

void EffectGravityGun::OnTick()
{
	Vector3 vec;
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &vec);
	
	if (!bCoordValid || (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z))
	{
		return;
	}
	
	lastVec = vec;
	
	std::vector <Entity> entities = GetNearbyPeds(50);
	
	auto nearbyVehs = GetNearbyVehs(45);
	
	for (auto veh : nearbyVehs)
	{
		entities.push_back(veh);
	}
	
	auto props = GetNearbyProps(45);
	
	for (auto prop : props)
	{
		ENTITY::SET_ENTITY_DYNAMIC(prop, true);
		ENTITY::SET_ENTITY_HAS_GRAVITY(prop, true);
		entities.push_back(prop);
	}
	
	Vector3 pVec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	
	for (auto entity : entities)
	{
		Vector3 vec2 = ENTITY::GET_ENTITY_COORDS(entity, true, 0);
		float dist = GetDistance3D(vec, vec2);
		
		const float maxDist = 5.0f;
		
		if (dist >= maxDist)
		{
			continue;
		}
		
		if (ENTITY::IS_ENTITY_A_PED(entity))
		{
			PED::SET_PED_TO_RAGDOLL(entity, 5000, 5000, 0, true, true, false);
		}
		
		float scale = 15.0f * (1.0f - (dist / maxDist));
		
		Vector3 diff = vec2;
		diff.x -= pVec.x;
		diff.y -= pVec.y;
		diff.z -= pVec.z;
		
		const float squareSum = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
		const float length = sqrt(squareSum);
		diff.x /= length;
		diff.y /= length;
		diff.z /= length;
		
		diff.x *= scale;
		diff.y *= scale;
		diff.y *= scale;
		
		ENTITY::SET_ENTITY_VELOCITY(entity, diff.x, diff.y, diff.z);
	}
}

void EffectDisableDeadEye::OnActivate()
{
	PLAYER::_0x95EE1DEE1DCD9070(PLAYER::PLAYER_ID(), false);
}

void EffectDisableDeadEye::OnDeactivate()
{
	PLAYER::_0x95EE1DEE1DCD9070(PLAYER::PLAYER_ID(), true);
}

void EffectSetRandomVelocity::OnActivate()
{
	Entity entity = PLAYER::PLAYER_PED_ID();
	
	bool bInVehicle = false;
	
	if (PED::IS_PED_IN_ANY_VEHICLE(entity, true))
	{
		entity = PED::GET_VEHICLE_PED_IS_IN(entity, false);
		bInVehicle = true;
	}
	else if (PED::IS_PED_ON_MOUNT(entity))
	{
		entity = PED::GET_MOUNT(entity);
	}
	
	
	if (!bInVehicle)
	{
		FixEntityInCutscene(entity);
		
		WAIT(75);
		PED::SET_PED_TO_RAGDOLL(entity, 2000, 2000, 0, true, true, false);
	}
	
	Vector3 vel;
	
	vel.x = float((rand() % 15) + 5);
	vel.y = float((rand() % 15) + 5);
	vel.z = float((rand() % 10) + 0);
	
	vel.x *= rand() % 2 ? -1 : 1;
	vel.y *= rand() % 2 ? -1 : 1;
	
	ENTITY::SET_ENTITY_VELOCITY(entity, vel.x, vel.y, vel.y);
}

void EffectFirstPerson::OnTick()
{
	CAM::_0x90DA5BA5C2635416();
}

void EffectTopDownCamera::OnActivate()
{
	this->cam = CAM::CREATE_CAM((char*) "DEFAULT_SCRIPTED_CAMERA", 1);
	CAM::RENDER_SCRIPT_CAMS(true, true, 500, 1, 1, 1);
	
	/** Should remove reticle */
	UI::_0x4CC5F2FC1332577F(GET_HASH("HUD_CTX_IN_FAST_TRAVEL_MENU"));
}

void EffectTopDownCamera::OnTick()
{
	CAM::SET_CAM_ACTIVE(this->cam, true);
	
	Vector3 camCoord = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 camRotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	
	vec.z += 10.0f;
	
	CAM::SET_CAM_ROT(cam, -90.0f, 0.0f, 0.0f, 2);
	CAM::SET_CAM_COORD(cam, vec.x, vec.y, vec.z);
	CAM::SET_CAM_AFFECTS_AIMING(cam, false);
}

void EffectTopDownCamera::OnDeactivate()
{
	CAM::SET_CAM_ACTIVE(this->cam, false);
	CAM::RENDER_SCRIPT_CAMS(false, true, 700, 1, 1, 1);
	CAM::SET_CAM_AFFECTS_AIMING(cam, true);
	CAM::DESTROY_CAM(this->cam, true);
	this->cam = 0;
	UI::_0x8BC7C1F929D07BF3(GET_HASH("HUD_CTX_IN_FAST_TRAVEL_MENU"));
}

void EffectAgitateHorse::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		Ped mount = PED::GET_MOUNT(playerPed);
		PED::_0xBAE08F00021BFFB2(mount, true);
	}
}

void EffectTpRandomLocation::OnActivate()
{
	static std::vector <std::vector<float>> coords = {{-301.0f,  790.0f,   119.0f}, /** Valentine */
													  {-1303.0f, 395.0f,   96.0f}, /** Wallace station */
													  {-1790.0f, -372.5f,  160.0f}, /** Strawberry */
													  {2432.8f,  -1216.0f, 46.0f}, /** Saint Denis */
													  {1526.5f,  431.0f,   91.0f}, /** Emerald Station */
													  {1264.4f,  -1311.0f, 77.0f}, /** Rhoads */
													  {2958.6f,  518.0f,   45.0f}, /** Van Horn */
													  {2927.0f,  1325.9f,  44.0f} /** Annesburg */
	};
	
	auto randomCoord = coords[rand() % coords.size()];
	
	if (randomCoord.size() != 3)
	{
		return;
	}
	
	TeleportPlayerTo(randomCoord[0], randomCoord[1], randomCoord[2]);
}

void EffectFakeTeleport::OnActivate()
{
	oldName = this->name;
	
	auto* chaosMod = ChaosMod::Singleton;
	
	if (chaosMod)
	{
		auto* originalEffect = chaosMod->EffectsMap["tp_to_random"];
		
		if (originalEffect)
		{
			this->name = originalEffect->name;
		}
	}
	
	bTeleportedBack = false;
	Effect::OnActivate();
	oldCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, 0);
	EffectTpRandomLocation::OnActivate();
}

void EffectFakeTeleport::OnTick()
{
	if (TimerTick(7000) && !bTeleportedBack)
	{
		TeleportToOldCoord();
		bTeleportedBack = true;
	}
}

void EffectFakeTeleport::OnDeactivate()
{
	if (!bTeleportedBack)
	{
		TeleportToOldCoord();
	}
	
	bTeleportedBack = false;
}

void EffectFakeTeleport::TeleportToOldCoord()
{
	TeleportPlayerTo(oldCoord.x, oldCoord.y, oldCoord.z);
	this->name = oldName;
	bTeleportedBack = true;
}
