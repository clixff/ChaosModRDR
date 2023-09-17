#include "peds.h"
#include "../script.h"
#include "misc.h"
#include "player.h"

void SetPedOnMount(Ped ped, Ped mount, int seat)
{
	/** __SET_PED_ON_MOUNT */
	invoke<Void>(0x028F76B6E78246EB, ped, mount, seat, true);
}

Ped SpawnPedAroundPlayer(Hash skinModel, bool bSetInVehicle, bool bSpawnHorseForPed)
{
	LoadModel(skinModel);
	
	/** IS_MODEL_A_HORSE */
	bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, skinModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	
	bool bPlayerInVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, true);
	
	if (bPlayerInVehicle)
	{
		playerLocation.z += 2.0f;
	}
	
	Ped ped = PED::CREATE_PED(skinModel, playerLocation.x, playerLocation.y, playerLocation.z, 0.0f, 1, 0, 0, 0);
	DECORATOR::DECOR_SET_INT(ped, (char*) "honor_override", 0);
	PED::SET_PED_VISIBLE(ped, true);
	PED::SET_PED_HEARING_RANGE(ped, 10000.0f);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(skinModel);
	
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		ChaosMod::pedsSet.insert(ped);
	}
	
	if (bSetInVehicle && !bModelIsHorse)
	{
		if (bPlayerInVehicle)
		{
			Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
			
			PED::SET_PED_INTO_VEHICLE(ped, playerVehicle, -2);
		}
		else if (PED::IS_PED_ON_MOUNT(playerPed))
		{
			Ped mount = PED::GET_MOUNT(playerPed);
			bool bIsMountSeatFree = invoke<bool>(0xAAB0FE202E9FC9F0, mount, 0);
			
			if (bIsMountSeatFree)
			{
				SetPedOnMount(ped, mount, 0);
			}
		}
	}
	
	if (bSpawnHorseForPed && !bModelIsHorse)
	{
		if (PED::IS_PED_ON_MOUNT(playerPed) || bPlayerInVehicle)
		{
			static Hash mountModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Horse_Morgan_Bay");
			
			Ped mount = SpawnPedAroundPlayer(mountModel, false, false);
			
			SetPedOnMount(ped, mount, -1);
		}
	}
	
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, false, false);
	
	return ped;
}

void MarkPedAsCompanion(Ped ped)
{
	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*) "PLAYER");
	Hash companionGroup;
	
	PED::ADD_RELATIONSHIP_GROUP((char*) "_CHAOS_COMPANION", &companionGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, companionGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, playerGroup, companionGroup);
	
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, companionGroup);
	
	PED::SET_PED_AS_GROUP_MEMBER(ped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID()));
	
	/** BF_CanFightArmedPedsWhenNotArmed */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	/** BF_AlwaysFight */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);
	
	static Hash blipHash = GET_HASH("BLIP_STYLE_FRIENDLY");
	
	/** BLIP_ADD_FOR_ENTITY */
	Blip blip = RADAR::_0x23F74C2FDA6E7C61(blipHash, ped);
	
	static Hash blipModifier = GET_HASH("BLIP_MODIFIER_MP_COLOR_1");
	
	/** BLIP_ADD_MODIFIER */
	RADAR::_0x662D364ABF16DE2F(blip, blipModifier);
	
	/** Allow talk */
	PED::SET_PED_CONFIG_FLAG(ped, 130, 0);
}

void MarkPedAsEnemy(Ped ped)
{
	static std::vector <Hash> groups = {GET_HASH("PLAYER"), 0x8A33CDCF, // Civ Male
										0x3220F762, // Civ Female
										0x3D714F12, 0x915095B1, 0x2A318608, 0x9BA3F8C6, 0xD23F79CC, 0xF287AFC3,
										0x403647E5};
	
	Hash enemyGroup;
	PED::ADD_RELATIONSHIP_GROUP((char*) "_CHAOS_ENEMY", &enemyGroup);
	
	for (uint32_t i = 0; i < groups.size(); i++)
	{
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, groups[i]);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, groups[i], enemyGroup);
	}
	
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, enemyGroup);
	
	/** BF_CanFightArmedPedsWhenNotArmed */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	/** BF_AlwaysFight */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);
	/** BF_CanUseVehicles */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 1, true);
	/** BF_CanLeaveVehicle */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, true);
	
	AI::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 16);
}

void FixEntityInCutscene(Entity entity)
{
	Vector3 vec = ENTITY::GET_ENTITY_COORDS(entity, true, 0);
	ENTITY::SET_ENTITY_COORDS(entity, vec.x, vec.y, vec.z, 0, 0, 0, 0);
}

void EffectSpawnSoldier::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "s_m_m_army_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	MarkPedAsCompanion(ped);
	
	RemoveAllPedWeapons(ped);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_SNIPERRIFLE_ROLLINGBLOCK");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
}

void EffectSpawnDrunkardJon::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_GrizzledJon");
	
	Ped ped = SpawnPedAroundPlayer(skinModel, false, true);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
	
	MarkPedAsEnemy(ped);
	
	/** Set drunk */
	invoke<Void>(0x406CCF555B04FAD3, ped, true, 0.75f);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnLenny::OnActivate()
{
	Effect::OnActivate();
	
	static std::vector <Hash> skins = {GAMEPLAY::GET_HASH_KEY((char*) "CS_lenny"),
									   GAMEPLAY::GET_HASH_KEY((char*) "MSP_SALOON1_MALES_01"),
									   GAMEPLAY::GET_HASH_KEY((char*) "MSP_SALOON1_FEMALES_01")};
	
	int32_t skinID = rand() % skins.size();
	
	Ped ped = SpawnPedAroundPlayer(skins[skinID]);
	
	MarkPedAsCompanion(ped);
	
	RemoveAllPedWeapons(ped);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_RIFLE_SPRINGFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	
	int32_t outfitID = 0;
	
	switch (skinID)
	{
		case 0:
			outfitID = rand() % 13;
			invoke<Void>(0x77FF8D35EEC6BBC4, ped, rand() % 13, false);
			break;
		case 1:
		{
			static std::vector <int32_t> outfits = {0, 2, 5, 7, 9, 11, 13, 15, 17, 21, 23, 25, 27, 29, 31, 33, 35, 37,
													39, 41, 43};
			
			outfitID = outfits[rand() % outfits.size()];
			
			break;
		}
		case 2:
		{
			static std::vector <int32_t> outfits = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
			
			outfitID = outfits[rand() % outfits.size()];
			break;
		}
	}
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, outfitID, false);
	
}

void EffectSpawnChicken::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Chicken_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	MarkPedAsCompanion(ped);
}

void EffectSpawnWolfPack::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Wolf");
	
	for (int i = 0; i < 8; i++)
	{
		Ped ped = SpawnPedAroundPlayer(skinModel);
		
		MarkPedAsCompanion(ped);
	}
}

void EffectKidnapping::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_AberdeenPigFarmer");
	static Hash skinModel2 = GAMEPLAY::GET_HASH_KEY((char*) "CS_AberdeenSister");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	Ped sister = SpawnPedAroundPlayer(skinModel2);
	
	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*) "PLAYER");
	Hash relationshipGroup;
	
	PED::ADD_RELATIONSHIP_GROUP((char*) "_CHAOS_KIDNAPPERS", &relationshipGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, relationshipGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, playerGroup, relationshipGroup);
	
	auto playerGroupID = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID());
	
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(sister, relationshipGroup);
	
	/** BF_CanLeaveVehicle */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
	/** BF_CanLeaveVehicle */
	PED::SET_PED_COMBAT_ATTRIBUTES(sister, 3, false);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerCoord = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
	
	Entity vehicle = 0;
	
	static Hash wagonModel = GAMEPLAY::GET_HASH_KEY((char*) "WAGONPRISON01X");
	LoadModel(wagonModel);
	
	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	vehicle = VEHICLE::CREATE_VEHICLE(wagonModel, playerCoord.x, playerCoord.y, playerCoord.z, playerHeading, 0, 0, 0,
									  0);
	DECORATOR::DECOR_SET_BOOL(vehicle, (char*) "wagon_block_honor", true);
	Vehicle vehCopy = vehicle;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);
	PED::SET_PED_INTO_VEHICLE(ped, vehicle, -1);
	PED::SET_PED_INTO_VEHICLE(sister, vehicle, 0);
	PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, 1);
	
	if (ENTITY::DOES_ENTITY_EXIST(vehicle))
	{
		ChaosMod::vehsSet.insert(vehicle);
	}
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(wagonModel);
	
	if (vehicle)
	{
		AI::TASK_VEHICLE_DRIVE_WANDER(ped, vehicle, 100000.0f, 0x400c0025);
		PED::SET_PED_KEEP_TASK(ped, true);
	}
}

void EffectKidnapping::OnTick()
{
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_VEH_EXIT"), true);
}

void EffectSpawnHorse::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Horse_Arabian_White");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	SetPedOnMount(playerPed, horse, -1);
}


std::vector <Ped> GetNearbyPeds(int32_t Max)
{
	std::vector <Ped> pedsOut;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	NearbyEntities peds;
	peds.size = 100;
	
	if (Max > 100)
	{
		Max = 100;
	}
	
	int pedsFound = PED::GET_PED_NEARBY_PEDS(playerPed, (int*) &peds, -1, 0);
	
	if (Max > pedsFound)
	{
		Max = pedsFound;
	}
	
	for (int32_t i = 0; i < Max; i++)
	{
		Entity ped = peds.entities[i];
		if (ENTITY::DOES_ENTITY_EXIST(ped) && ped != playerPed)
		{
			pedsOut.push_back(ped);
		}
	}
	
	return pedsOut;
}

void RemovePedFromVeh(Ped ped)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(ped, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(veh, true, 0);
		
		ENTITY::SET_ENTITY_COORDS(ped, vec.x, vec.y, vec.z + 2.0f, 0, 0, 0, 0);
	}
	else if (PED::IS_PED_ON_MOUNT(ped))
	{
		/** _REMOVE_PED_FROM_MOUNT */
		invoke<Void>(0x5337B721C51883A9, ped, 0, 0);
	}
}

void RemoveAllPedWeapons(Ped ped)
{
	static Hash unarmed = GET_HASH("WEAPON_UNARMED");
	WEAPON::SET_CURRENT_PED_WEAPON(ped, unarmed, 1, 0, 0, 0);
	
	/** This weapons can't be received without mods, so don't remove it */
	static std::set <Hash> importantWeapons = {GET_HASH("WEAPON_BOW"), GET_HASH("WEAPON_KIT_CAMERA"),
											   GET_HASH("WEAPON_LASSO"), GET_HASH("WEAPON_FISHINGROD"),
											   GET_HASH("WEAPON_MELEE_LANTERN_ELECTRIC"),
											   GET_HASH("WEAPON_MELEE_KNIFE")};
	
	for (Hash weaponHash : Effect::WeaponHashes)
	{
		if (importantWeapons.contains(weaponHash))
		{
			continue;
		}
		
		WEAPON::REMOVE_WEAPON_FROM_PED(ped, weaponHash, 0, 0);
	}
}

void EffectSpawnMule::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_HorseMule_01");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnDonkey::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Donkey_01");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnSerialKiller::OnActivate()
{
	/** Spawn donkey */
	
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Donkey_01");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** Spawn killer */
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "G_M_M_UniDuster_03");
	
	Ped ped = SpawnPedAroundPlayer(pedModel);
	
	SetPedOnMount(ped, horse, -1);
	
	MarkPedAsEnemy(ped);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnVampire::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_Vampire");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
	
	MarkPedAsEnemy(ped);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnGiantDonkey::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Donkey_01");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, horse, 1.5f);
	
	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnMiniDonkey::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Donkey_01");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, horse, 0.42f);
	
	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnGiantCop::OnActivate()
{
	Effect::OnActivate();
	
	static Hash copGroup = GAMEPLAY::GET_HASH_KEY((char*) "COP");
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "S_M_M_DispatchLeaderPolice_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel, false, false);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 10.0f);
	
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, copGroup);
	
	AI::TASK_LOOK_AT_ENTITY(ped, PLAYER::PLAYER_PED_ID(), -1, 2048, 3, 1);
	AI::TASK_WANDER_STANDARD(ped, 10.0f, 10);
}

void EffectSpawnAngrySkeleton::OnActivate()
{
	Effect::OnActivate();
	
	static std::vector<const char*> skins = {"U_M_M_CircusWagon_01",
			//"CS_ODPROSTITUTE",
											 "A_M_M_UniCorpse_01", "A_F_M_UniCorpse_01"};
	
	uint32_t skinID = rand() % skins.size();
	
	Ped ped = SpawnPedAroundPlayer(GAMEPLAY::GET_HASH_KEY((char*) skins[skinID]));
	
	if (skinID == 1)
	{
		static std::vector <uint32_t> skeletonOutfitsIDs = {31, 36, 46, 56, 68, 143, 144, 147,};
		
		/** Set outfit */
		invoke<Void>(0x77FF8D35EEC6BBC4, ped, skeletonOutfitsIDs[rand() % skeletonOutfitsIDs.size()], false);
	}
	else if (skinID == 2)
	{
		static std::vector <uint32_t> skeletonOutfitsIDs = {11, 12, 27, 28,};
		
		/** Set outfit */
		invoke<Void>(0x77FF8D35EEC6BBC4, ped, skeletonOutfitsIDs[rand() % skeletonOutfitsIDs.size()], false);
	}
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);
	
	if (rand() % 2 == 0)
	{
		static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_MELEE_BROKEN_SWORD");
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 1, true, 0x2cd419dc);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	}
	
	MarkPedAsEnemy(ped);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnAngryDwarf::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Donkey_01");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, horse, 0.5f);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** Spawn dwarf */
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_Magnifico");
	
	Ped ped = SpawnPedAroundPlayer(pedModel, false);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_MELEE_KNIFE");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	
	SetPedOnMount(ped, horse, -1);
	
	MarkPedAsEnemy(ped);
	
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && !PED::IS_PED_ON_MOUNT(playerPed))
	{
	
	}
	
	/** _REMOVE_PED_FROM_MOUNT */
	invoke<Void>(0x5337B721C51883A9, ped, 0, 0);
}

void EffectSpawnCompanionBertram::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_ODDFELLOWSPINHEAD");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 1.25f);
	
	MarkPedAsCompanion(ped);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
	
	WEAPON::REMOVE_ALL_PED_WEAPONS(ped, true, 0);
}

void EffectSpawnFrozenCouple::OnActivate()
{
	Effect::OnActivate();
	
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "RE_FROZENTODEATH_FEMALES_01");
	static Hash pedModel2 = GAMEPLAY::GET_HASH_KEY((char*) "RE_FROZENTODEATH_MALES_01");
	
	Ped ped = SpawnPedAroundPlayer(pedModel);
	Ped ped2 = SpawnPedAroundPlayer(pedModel2);
	
	MarkPedAsEnemy(ped);
	MarkPedAsEnemy(ped2);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped2, 263, true);
}

void EffectSpawnRobot::OnActivate()
{
	Effect::OnActivate();
	
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_crackpotRobot");
	
	Ped ped = SpawnPedAroundPlayer(pedModel);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 10000);
	ENTITY::SET_ENTITY_HEALTH(ped, 10000, 0);
	
	MarkPedAsCompanion(ped);
}

void EffectSpawnLassoGuy::OnActivate()
{
	Effect::OnActivate();
	
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_EXOTICCOLLECTOR");
	
	Ped ped = SpawnPedAroundPlayer(pedModel, false, true);
	
	MarkPedAsEnemy(ped);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_LASSO");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** TASK_LASSO_PED */
	invoke<Void>(0xC716EB2BD16370A3, ped, PLAYER::PLAYER_PED_ID());
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSkyrimIntro::OnActivate()
{
	Effect::OnActivate();
	
	GRAPHICS::ANIMPOSTFX_PLAY((char*) "PlayerWakeUpInterrogation");
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerCoord = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	
	static Hash copModel = GAMEPLAY::GET_HASH_KEY((char*) "U_M_O_BlWPoliceChief_01");
	static Hash wagonModel = GAMEPLAY::GET_HASH_KEY((char*) "wagon03x");
	static Hash prisonerModel1 = GAMEPLAY::GET_HASH_KEY((char*) "RE_PRISONWAGON_MALES_01");
	static Hash prisonerModel2 = GAMEPLAY::GET_HASH_KEY((char*) "RE_LONEPRISONER_MALES_01");
	static Hash prisonerModel3 = GAMEPLAY::GET_HASH_KEY((char*) "CS_chainprisoner_01");
	
	Ped cop = SpawnPedAroundPlayer(copModel, false);
	Ped prisoner1 = SpawnPedAroundPlayer(prisonerModel1, false);
	Ped prisoner2 = SpawnPedAroundPlayer(prisonerModel2, false);
	Ped prisoner3 = SpawnPedAroundPlayer(prisonerModel3, false);
	
	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*) "PLAYER");
	Hash relationshipGroup;
	
	PED::ADD_RELATIONSHIP_GROUP((char*) "_CHAOS_WAGON", &relationshipGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, relationshipGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, playerGroup, relationshipGroup);
	
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(cop, relationshipGroup);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(prisoner1, relationshipGroup);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(prisoner2, relationshipGroup);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(prisoner3, relationshipGroup);
	
	/** BF_CanLeaveVehicle */
	PED::SET_PED_COMBAT_ATTRIBUTES(cop, 3, false);
	PED::SET_PED_COMBAT_ATTRIBUTES(prisoner1, 3, false);
	PED::SET_PED_COMBAT_ATTRIBUTES(prisoner2, 3, false);
	PED::SET_PED_COMBAT_ATTRIBUTES(prisoner3, 3, false);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_REPEATER_WINCHESTER");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(cop, weaponHash, 100, true, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(cop, weaponHash, 100);
	
	LoadModel(wagonModel);
	
	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(wagonModel, playerCoord.x, playerCoord.y, playerCoord.z, playerHeading, 0,
											  0, 0, 0);
	
	Vehicle vehCopy = vehicle;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);
	
	PED::SET_PED_INTO_VEHICLE(cop, vehicle, -1);
	PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, 1);
	PED::SET_PED_INTO_VEHICLE(prisoner1, vehicle, 2);
	PED::SET_PED_INTO_VEHICLE(prisoner2, vehicle, 3);
	PED::SET_PED_INTO_VEHICLE(prisoner3, vehicle, 4);
	
	if (ENTITY::DOES_ENTITY_EXIST(vehicle))
	{
		ChaosMod::vehsSet.insert(vehicle);
	}
	
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle, 0);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(wagonModel);
	
	AI::TASK_VEHICLE_DRIVE_WANDER(cop, vehicle, 10000.0f, 0x400c0025);
	PED::SET_PED_KEEP_TASK(cop, true);
}

void EffectSkyrimIntro::OnTick()
{
	CONTROLS::DISABLE_CONTROL_ACTION(0, GET_HASH("INPUT_VEH_EXIT"), true);
	CAM::_0x90DA5BA5C2635416();
}

void EffectSpawnParrotCompanion::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Parrot_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	MarkPedAsCompanion(ped);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && !PED::IS_PED_ON_MOUNT(playerPed))
	{
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
		
		vec.z += 2.0f;
		
		ENTITY::SET_ENTITY_COORDS(ped, vec.x, vec.y, vec.z, false, false, false, false);
	}
}

void EffectSpawnShireHorse::OnActivate()
{
	Effect::OnActivate();
	
	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Horse_Shire_DarkBay");
	
	Ped horse = SpawnPedAroundPlayer(horseModel);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	SetPedOnMount(playerPed, horse, -1);
}

void EffectUndeadNightmare::OnActivate()
{
	Effect::OnActivate();
	
	TIME::SET_CLOCK_TIME(22, 0, 0);
	
	static Hash weatherHash = GAMEPLAY::GET_HASH_KEY((char*) "FOG");
	
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	GAMEPLAY::SET_WEATHER_TYPE(weatherHash, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	
	static Hash skin = GAMEPLAY::GET_HASH_KEY((char*) "A_M_M_ARMCHOLERACORPSE_01");
	static Hash skin2 = GAMEPLAY::GET_HASH_KEY((char*) "A_M_M_UniCorpse_01");
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	Vector3 playerVec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	
	for (int32_t i = 0; i < 5; i++)
	{
		Ped zombie = SpawnPedAroundPlayer(i == 0 ? skin : skin2, false);
		
		if (i != 0)
		{
			/** Set outfit */
			invoke<Void>(0x77FF8D35EEC6BBC4, zombie, rand() % 187, false);
		}
		
		float radius = 10.0f;
		
		Vector3 coord = playerVec;
		
		/** In radians */
		float angle = i * (360.0f / 5) * (M_PI / 180.0f);
		
		coord.x += radius * sin(angle);
		coord.y += radius * cos(angle);
		
		ENTITY::SET_ENTITY_COORDS(zombie, coord.x, coord.y, coord.z, false, false, false, false);
		
		/** Set walking style */
		invoke<Void>(0x89F5E7ADECCCB49C, zombie, "very_drunk");
		
		MarkPedAsEnemy(zombie);
		
		/** BF_CanUseVehicles */
		PED::SET_PED_COMBAT_ATTRIBUTES(zombie, 1, false);
		
		static Hash blipHash = GET_HASH("BLIP_STYLE_ENEMY");
		
		/** BLIP_ADD_FOR_ENTITY */
		Blip blip = RADAR::_0x23F74C2FDA6E7C61(blipHash, zombie);
	}
}

void EffectSpawnDogCompanion::OnActivate()
{
	Effect::OnActivate();
	
	static std::vector<const char*> dogModels = {"A_C_DogAmericanFoxhound_01", "A_C_DogAustralianSheperd_01",
												 "A_C_DogBluetickCoonhound_01", "A_C_DogCatahoulaCur_01",
												 "A_C_DogChesBayRetriever_01", "A_C_DogCollie_01", "A_C_DogHobo_01",
												 "A_C_DogHound_01", "A_C_DogHusky_01"};
	
	Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) dogModels[rand() % dogModels.size()]);
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	MarkPedAsCompanion(ped);
}

void EffectSpawnCatCompanion::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Cat_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	MarkPedAsCompanion(ped);
}

void EffectSpawnBearCompanion::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_BearBlack_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	MarkPedAsCompanion(ped);
	
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, false);
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);
}

void EffectSpawnAngryCorpse::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_M_M_ARMCHOLERACORPSE_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 300);
	ENTITY::SET_ENTITY_HEALTH(ped, 300, 0);
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 13, false);
	
	MarkPedAsEnemy(ped);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnAngryCaveman::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_M_M_UniCorpse_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 44, false);
	
	MarkPedAsEnemy(ped);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 1.1f);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnAngryTwin::OnActivate()
{
	Effect::OnActivate();
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	Ped ped = PED::CLONE_PED(playerPed, ENTITY::GET_ENTITY_HEADING(playerPed), true, false);
	
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		ChaosMod::pedsSet.insert(ped);
	}
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		
		PED::SET_PED_INTO_VEHICLE(ped, veh, -2);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		Ped mount = PED::GET_MOUNT(playerPed);
		
		bool bIsMountSeatFree = invoke<bool>(0xAAB0FE202E9FC9F0, mount, 0);
		
		if (bIsMountSeatFree)
		{
			SetPedOnMount(ped, mount, 0);
		}
	}
	
	ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
	ENTITY::SET_ENTITY_PROOFS(ped, false, false);
	
	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_UNARMED");
	Hash weaponHash = 0;
	
	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, 0, 0, 0) && weaponHash != unarmed)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(ped, weaponHash, 100);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);
	}
	
	MarkPedAsEnemy(ped);
}

void EffectSpawnAngryCowboy::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "S_M_M_ValCowpoke_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel, false, true);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, rand() % 22, false);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_REVOLVER_SCHOFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 200, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(ped, weaponHash, 200);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	MarkPedAsEnemy(ped);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectSpawnUndeadBoss::OnActivate()
{
	Effect::OnActivate();
	
	TIME::SET_CLOCK_TIME(22, 0, 0);
	
	static Hash weatherHash = GAMEPLAY::GET_HASH_KEY((char*) "FOG");
	
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	GAMEPLAY::SET_WEATHER_TYPE(weatherHash, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "A_M_M_UniCorpse_01");
	
	Ped ped = SpawnPedAroundPlayer(skinModel, false, false);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 700);
	ENTITY::SET_ENTITY_HEALTH(ped, 700, 0);
	
	/** Set outfit */
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, rand() % 2 ? 68 : 45, false);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 1.6f);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_THROWN_TOMAHAWK_ANCIENT");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, true, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(ped, weaponHash, 100);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	
	/** SET_ALLOW_ANY_WEAPON_DROP */
	WEAPON::_0x78030C7867D8B9B6(ped, false);
	
	Entity weaponEntity = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(ped, 0);
	/** Set walking style */
	invoke<Void>(0x89F5E7ADECCCB49C, ped, "injured_right_leg");
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
	
	/** PCF_DisableAllMeleeTakedowns */
	PED::SET_PED_CONFIG_FLAG(ped, 340, true);
	
	PED::_SET_PED_RAGDOLL_BLOCKING_FLAGS(ped, 1 | 2 | 16 | 128 | 512 | 8192);
	
	MarkPedAsEnemy(ped);
	
	PED::_0x1902C4CFCC5BE57C(ped, 0x48760F4A);
	PED::_0xCC8CA3E88256E58F(ped, 0, 1, 1, 1, false);
}

void EffectSpawnGrieferMicah::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_MicahBell");
	
	Ped ped = SpawnPedAroundPlayer(skinModel, false, true);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 10, false);
	
	int randNum = rand() % 2;
	
	Hash weaponHash = randNum ? GET_HASH("WEAPON_THROWN_DYNAMITE") : GET_HASH("WEAPON_THROWN_MOLOTOV");
	
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 20, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(ped, weaponHash, 20);
	
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
	
	if (PED::IS_PED_ON_MOUNT(ped))
	{
		static Hash weaponHashRevolver = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_REVOLVER_SCHOFIELD_GOLDEN");
		
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHashRevolver, 100, 1, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(ped, weaponHashRevolver, 100);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHashRevolver, 1, 0, 0, 0);
	}
	
	MarkPedAsEnemy(ped);
}

void EffectDutchStealsPlayersVeh::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	bool bUsingVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, true);
	bool bUsingHorse = PED::IS_PED_ON_MOUNT(playerPed);
	
	if (!bUsingHorse && !bUsingVehicle)
	{
		return;
	}
	
	static Hash model = GET_HASH("CS_Dutch");
	
	Ped ped = SpawnPedAroundPlayer(model, false, false);
	
	MarkPedAsCompanion(ped);
	
	if (bUsingHorse)
	{
		Ped mount = PED::GET_MOUNT(playerPed);
		
		bool bIsMountSeatFree = invoke<bool>(0xAAB0FE202E9FC9F0, mount, 0);
		
		if (bIsMountSeatFree)
		{
			SetPedOnMount(playerPed, mount, 0);
		}
		else
		{
			/** _REMOVE_PED_FROM_MOUNT */
			invoke<Void>(0x5337B721C51883A9, playerPed, 0, 0);
		}
		
		SetPedOnMount(ped, mount, -1);
		
		AI::TASK_WANDER_STANDARD(ped, 100.0f, 10);
	}
	else
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		
		Hash vehModel = ENTITY::GET_ENTITY_MODEL(veh);
		
		if (!VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1))
		{
			Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1);
			
			int32_t seatsNum = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(vehModel) - 1;
			
			bool bFoundNewSeatForDriver = false;
			
			for (int32_t i = 0; i < seatsNum; i++)
			{
				bool bSeatFree = VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i);
				
				if (bSeatFree)
				{
					PED::SET_PED_INTO_VEHICLE(driver, veh, i);
					bFoundNewSeatForDriver = true;
					break;
				}
			}
			
			if (!bFoundNewSeatForDriver)
			{
				Vector3 vec = ENTITY::GET_ENTITY_COORDS(veh, true, 0);
				
				ENTITY::SET_ENTITY_COORDS(driver, vec.x, vec.y, vec.z + 1.5f, false, false, false, false);
			}
		}
		
		PED::SET_PED_INTO_VEHICLE(ped, veh, -1);
		
		AI::TASK_VEHICLE_DRIVE_WANDER(ped, veh, 100000.0f, 0x400c0025);
	}
	
	PED::SET_PED_KEEP_TASK(ped, true);
	
	this->dutch = ped;
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 15, false);
}

void EffectDutchStealsPlayersVeh::OnDeactivate()
{
	if (ENTITY::DOES_ENTITY_EXIST(dutch))
	{
		PED::REMOVE_PED_FROM_GROUP(dutch);
	}
}

void EffectSpawnPredator::OnActivate()
{
	static std::vector<const char*> models = {"A_C_Alligator_01", "A_C_Bear_01", "A_C_Cougar_01", "A_C_LionMangy_01",
											  "A_C_Panther_01", "A_C_Wolf"};
	
	std::map<const char*, std::set < uint32_t>>
	disabledOutfits;
	
	disabledOutfits.emplace("A_C_Bear_01", std::set<uint32_t>({9, 10}));
	
	auto modelName = models[rand() % models.size()];
	
	Ped ped = SpawnPedAroundPlayer(GET_HASH(modelName), false, false);
	
	uint32_t maxOutfits = PED::_0x10C70A515BC03707(ped);
	
	uint32_t randOutfit = rand() % maxOutfits;
	
	if (disabledOutfits.contains(modelName))
	{
		if (disabledOutfits[modelName].contains(randOutfit))
		{
			randOutfit = 0;
		}
	}
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, randOutfit, false);
	
	MarkPedAsEnemy(ped);
}

void EffectPedsBhop::OnTick()
{
	if (!TimerTick(500))
	{
		return;
	}
	
	auto nearbyPeds = GetNearbyPeds(50);
	
	for (auto ped : nearbyPeds)
	{
		if (!PED::IS_PED_HUMAN(ped))
		{
			continue;
		}
		
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, true))
		{
			Vector3 vec = ENTITY::GET_ENTITY_COORDS(ped, true, 0);
			
			ENTITY::SET_ENTITY_COORDS(ped, vec.x, vec.y, vec.z + 1.5f, false, false, false, false);
		}
		else if (PED::IS_PED_ON_MOUNT(ped))
		{
			/** _REMOVE_PED_FROM_MOUNT */
			invoke<Void>(0x5337B721C51883A9, ped, 0, 0);
		}
		else
		{
			AI::TASK_JUMP(ped, 0);
		}
	}
}

void EffectPedsSpin::OnActivate()
{
	peds.clear();
	heading = 0.0f;
}

void EffectPedsSpin::OnTick()
{
	if (TimerTick(500))
	{
		peds.clear();
		
		auto nearbyPeds = GetNearbyPeds(50);
		
		for (auto ped : nearbyPeds)
		{
			peds.insert(ped);
		}
	}
	
	heading += 625.0f * ChaosMod::GetDeltaTimeSeconds();
	
	heading = fmod(heading, 360.0f);
	
	for (auto ped : peds)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, true) || PED::IS_PED_ON_MOUNT(ped))
		{
			continue;
		}
		
		ENTITY::SET_ENTITY_HEADING(ped, heading);
	}
}

void EffectCloneEnemy::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	auto nearbyPeds = GetNearbyPeds(50);
	std::vector <Ped> validPeds;
	
	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			int relationships = PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, playerPed);
			
			if (relationships == 5)
			{
				validPeds.push_back(ped);
			}
		}
	}
	
	if (!validPeds.size())
	{
		return;
	}
	
	Ped ped = validPeds[rand() % validPeds.size()];
	
	Ped clone = PED::CLONE_PED(ped, ENTITY::GET_ENTITY_HEADING(ped), false, false);
	
	if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
		
		if (VEHICLE::ARE_ANY_VEHICLE_SEATS_FREE(veh))
		{
			PED::SET_PED_INTO_VEHICLE(clone, veh, -2);
		}
	}
	if (PED::IS_PED_ON_MOUNT(ped))
	{
		Ped mount = PED::GET_MOUNT(ped);
		
		bool bIsMountSeatFree = invoke<bool>(0xAAB0FE202E9FC9F0, mount, 0);
		
		if (bIsMountSeatFree)
		{
			SetPedOnMount(clone, mount, 0);
		}
	}
	
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);
	
	MarkPedAsEnemy(clone);
	
	if (ENTITY::DOES_ENTITY_EXIST(clone))
	{
		ChaosMod::pedsSet.insert(clone);
	}
}

void EffectPedsFollowPlayer::OnActivate()
{
	peds.clear();
}

void EffectPedsFollowPlayer::OnTick()
{
	if (!TimerTick(1000))
	{
		return;
	}
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	auto nearbyPeds = GetNearbyPeds(50);
	
	for (auto ped : nearbyPeds)
	{
		if (peds.contains(ped) || ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped))
		{
			continue;
		}
		
		AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped, playerPed, 0.0f, 0.0f, 0.0f, 4.5f, -1.0f, -1.0f, 0, 0, 0, 0, 0);
		peds.insert(ped);
	}
}

void EffectPedsFollowPlayer::OnDeactivate()
{
	for (auto ped : peds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			AI::CLEAR_PED_TASKS_IMMEDIATELY(ped, true, true);
			AI::TASK_WANDER_STANDARD(ped, 10.0f, 10);
		}
	}
	
	peds.clear();
}

void EffectPedsFleeing::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(50);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	for (auto ped : nearbyPeds)
	{
		if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped))
		{
			continue;
		}
		
		invoke<Void>(0x22B0D0E37CCB840D, ped, playerPed, 70.0f, 10000.0f, 0, 3.0f, 0);
	}
}

void EffectHealNearbyPeds::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(45);
	
	for (auto ped : nearbyPeds)
	{
		int maxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(ped, 0);
		ENTITY::SET_ENTITY_HEALTH(ped, maxHealth, 0);
	}
}

void EffectReviveDeadPeds::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int* worldPeds = new int[150];
	
	int found = worldGetAllPeds(worldPeds, 150);
	
	for (int32_t i = 0; i < found; i++)
	{
		Ped ped = worldPeds[i];
		if (ENTITY::DOES_ENTITY_EXIST(ped) && ped != playerPed)
		{
			if (PED::IS_PED_DEAD_OR_DYING(ped, 1))
			{
				int relationships = PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, playerPed);
				
				int health = ENTITY::GET_ENTITY_MAX_HEALTH(ped, 0);
				
				PED::RESURRECT_PED(ped);
				
				ENTITY::SET_ENTITY_MAX_HEALTH(ped, health);
				ENTITY::SET_ENTITY_HEALTH(ped, health, 0);
				
				PED::REVIVE_INJURED_PED(ped);
				
				AI::CLEAR_PED_TASKS_IMMEDIATELY(ped, 0, true);
				
				MarkPedAsEnemy(ped);
				
				PED::SET_PED_TO_RAGDOLL(ped, 500, 500, 0, true, true, false);
				
			}
			if (PED::IS_PED_INJURED(ped))
			{
				PED::REVIVE_INJURED_PED(ped);
			}
		}
	}
	
	delete[] worldPeds;
}

void EffectSpawnAngryTommy::OnActivate()
{
	Effect::OnActivate();
	
	static Hash skinModel = GET_HASH("CS_mud2bigguy");
	
	Ped ped = SpawnPedAroundPlayer(skinModel, false, true);
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
	
	MarkPedAsEnemy(ped);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 1.1f);
	
	/** Set outfit */
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 1, false);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
}

void EffectPartyTime::OnActivate()
{
	peds.clear();
	pedAnimNames.clear();
}

void EffectPartyTime::OnTick()
{
	DisableAllMovements();
	
	if (!TimerTick(1000))
	{
		return;
	}
	
	static char* animDict = (char*) "script_shows@cancandance@p1";
	static std::vector<const char*> animNames = {"cancandance_fem0", "cancandance_fem1", "cancandance_fem2",
												 "cancandance_fem3", "cancandance_male"};
	
	STREAMING::REQUEST_ANIM_DICT(animDict);
	
	while (!STREAMING::HAS_ANIM_DICT_LOADED(animDict))
	{
		WAIT(0);
	}
	
	auto nearbyPeds = GetNearbyPeds(50);
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	nearbyPeds.push_back(playerPed);
	
	for (auto ped : nearbyPeds)
	{
		if (!PED::IS_PED_HUMAN(ped))
		{
			continue;
		}
		
		if (peds.contains(ped) && ENTITY::IS_ENTITY_PLAYING_ANIM(ped, animDict, pedAnimNames[ped], 1))
		{
			continue;
		}
		
		if (ped != playerPed && ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped))
		{
			int relationhips = PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, playerPed);
			
			if (relationhips < 4)
			{
				continue;
			}
		}
		
		RemovePedFromVeh(ped);
		
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped, 0, true);
		
		char* animName = (char*) (animNames[rand() % animNames.size()]);
		
		pedAnimNames.emplace(ped, animName);
		
		AI::TASK_PLAY_ANIM(ped, animDict, animName, 3.0f, -3.0f, -1, 1, 0, false, false, false, (Any*) "", 0);
		//PED::SET_PED_KEEP_TASK(ped, true);
		//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
		
		peds.insert(ped);
	}
	
	STREAMING::REMOVE_ANIM_DICT(animDict);
}

void EffectPartyTime::OnDeactivate()
{
	static char* animDict = (char*) "script_shows@cancandance@p1";
	
	STREAMING::REMOVE_ANIM_DICT(animDict);
	
	for (auto ped : peds)
	{
		char* animName = pedAnimNames[ped];
		if (ENTITY::DOES_ENTITY_EXIST(ped) && ENTITY::IS_ENTITY_PLAYING_ANIM(ped, animDict, animName, 1))
		{
			AI::STOP_ANIM_TASK(ped, animDict, animName, 0);
		}
	}
	
	peds.clear();
	pedAnimNames.clear();
}

void EffectExplodeNearbyPeds::OnActivate()
{
	const static int max = 15;
	
	auto peds = GetNearbyPeds(50);
	
	int i = 0;
	
	for (auto ped : peds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);
			
			/** IS_MODEL_A_HORSE */
			bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);
			if (bModelIsHorse)
			{
				continue;
			}
			
			Vector3 vec = ENTITY::GET_ENTITY_COORDS(ped, true, 0);
			
			FIRE::ADD_EXPLOSION(vec.x, vec.y, vec.z, 27, 1.0f, true, false, 1.0f);
			
			ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
			
			i++;
			
			if (i >= max)
			{
				return;
			}
		}
	}
}

void EffectNearbyPedIsCompanion::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(50);
	
	std::vector <Ped> peds;
	
	for (auto ped : nearbyPeds)
	{
		Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);
		
		/** IS_MODEL_A_HORSE */
		bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);
		bool bEntityIsBird = invoke<bool>(0xC346A546612C49A9, ped);
		
		if (!bModelIsHorse && !bEntityIsBird && !ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped))
		{
			peds.push_back(ped);
		}
	}
	
	if (!peds.size())
	{
		return;
	}
	
	Ped ped = peds[rand() % peds.size()];
	
	AI::CLEAR_PED_TASKS_IMMEDIATELY(ped, 0, true);
	
	MarkPedAsCompanion(ped);
	
	//AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped, PLAYER::PLAYER_PED_ID(), 0.0f, 0.0f, 0.0f, 4.5f, -1.0f, -1.0f, 0, 0, 0, 0, 0);
	
	if (PED::IS_PED_HUMAN(ped))
	{
		static Hash weaponHash = GET_HASH("WEAPON_REPEATER_CARBINE");
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(ped, weaponHash, 9999);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	}
}

void EffectEveryoneRagdollsWhenShot::OnActivate()
{
	peds.clear();
}

void EffectEveryoneRagdollsWhenShot::OnTick()
{
	if (TimerTick(500))
	{
		peds.clear();
		
		auto nearbyPeds = GetNearbyPeds(50);
		
		nearbyPeds.push_back(PLAYER::PLAYER_PED_ID());
		
		for (auto ped : nearbyPeds)
		{
			peds.insert(ped);
		}
	}
	
	for (auto ped : peds)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(ped) || ENTITY::IS_ENTITY_DEAD(ped))
		{
			continue;
		}
		
		Vector3 vec;
		bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(ped, &vec);
		
		if (bCoordValid)
		{
			PED::SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);
		}
	}
}

void EffectNearbyPedIsEnemy::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(50);
	
	std::vector <Ped> peds;
	
	for (auto ped : nearbyPeds)
	{
		if (PED::IS_PED_HUMAN(ped))
		{
			peds.push_back(ped);
		}
	}
	
	if (!peds.size())
	{
		return;
	}
	
	Ped ped = peds[rand() % peds.size()];
	
	if (PED::IS_PED_HUMAN(ped))
	{
		static Hash weaponHash = GET_HASH("WEAPON_REPEATER_CARBINE");
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(ped, weaponHash, 9999);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	}
	
	MarkPedAsEnemy(ped);
	
	static Hash blipHash = GET_HASH("BLIP_STYLE_ENEMY");
	
	/** BLIP_ADD_FOR_ENTITY */
	Blip blip = RADAR::_0x23F74C2FDA6E7C61(blipHash, ped);
}

void EffectExplosiveCombat::OnActivate()
{
	peds.clear();
}

void EffectExplosiveCombat::OnTick()
{
	if (TimerTick(500))
	{
		peds.clear();
		
		auto nearbyPeds = GetNearbyPeds(50);
		
		nearbyPeds.push_back(PLAYER::PLAYER_PED_ID());
		
		for (auto ped : nearbyPeds)
		{
			peds.insert(ped);
		}
	}
	
	for (auto ped : peds)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(ped) || ENTITY::IS_ENTITY_DEAD(ped))
		{
			continue;
		}
		
		Vector3 vec;
		bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(ped, &vec);
		
		if (bCoordValid)
		{
			FIRE::ADD_EXPLOSION(vec.x, vec.y, vec.z, 27, 1.0f, true, false, 1.0f);
		}
	}
}

void EffectODriscolls::OnActivate()
{
	static Hash model = GET_HASH("g_m_m_uniduster_01");
	static Hash horseModel = GET_HASH("A_C_Horse_Arabian_Black");
	
	Ped ped1 = SpawnPedAroundPlayer(model, false, false);
	Ped ped2 = SpawnPedAroundPlayer(model, false, false);
	
	std::vector <Ped> peds = {ped1, ped2};
	
	Ped mount = SpawnPedAroundPlayer(horseModel, false, false);
	SetPedOnMount(ped1, mount, -1);
	
	for (auto ped : peds)
	{
		static Hash weaponHash = GET_HASH("WEAPON_REPEATER_CARBINE");
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(ped, weaponHash, 9999);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
		/** Set outfit */
		invoke<Void>(0x77FF8D35EEC6BBC4, ped, rand() % 184, false);
		
		ENTITY::SET_ENTITY_MAX_HEALTH(ped, 300);
		ENTITY::SET_ENTITY_HEALTH(ped, 300, 0);
		
		/** PCF_NoCriticalHits */
		PED::SET_PED_CONFIG_FLAG(ped, 263, true);
		
		MarkPedAsEnemy(ped);
	}
}

void EffectBanditoKidnapsPlayer::OnActivate()
{
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "G_M_M_UNIBANDITOS_01");
	static Hash donkeyModel = GAMEPLAY::GET_HASH_KEY((char*) "A_C_Donkey_01");
	
	Ped donkey = SpawnPedAroundPlayer(donkeyModel, false, false);
	Ped ped = SpawnPedAroundPlayer(pedModel, false, false);
	
	SetPedOnMount(ped, donkey, -1);
	
	MarkPedAsEnemy(ped);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_LASSO");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	/** TASK_LASSO_PED */
	invoke<Void>(0xC716EB2BD16370A3, ped, PLAYER::PLAYER_PED_ID());
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
	
	/** Set outfit */
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 0, false);
}

void EffectSpawnTwitchViewer::OnActivate()
{
	Effect::OnActivate();
	
	bool bRequestSent = ChaosMod::Singleton->RequestTwitchViewerNameToSpawn();
	
	//if (!bRequestSent)
	//{
	//	Spawn("Viewer_Name");
	//}
}

void EffectSpawnTwitchViewer::Spawn(std::string name)
{
	static std::vector<const char*> skins = {"a_m_m_vallaborer_01", "a_m_m_valtownfolk_01", "a_m_m_valtownfolk_02",
											 "a_m_m_tumtownfolk_01", "a_f_m_valtownfolk_01", "a_m_m_asbtownfolk_01"};
	
	Hash model = GET_HASH(skins[rand() % skins.size()]);
	
	ped = SpawnPedAroundPlayer(model, false, true);
	
	uint32_t maxOutfits = PED::_0x10C70A515BC03707(ped);
	uint32_t randOutfit = rand() % maxOutfits;
	
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, randOutfit, false);
	
	static std::vector <Hash> faceIndices = {0x84D6, 0x3303, 0x2FF9, 0x4AD1, 0xC04F, 0xB6CE, 0x2844, 0xED30, 0x6A0B,
											 0xABCF, 0x358D, 0x8D0A, 0xEBAE, 0x1DF6, 0x3C0F, 0xC3B2, 0xE323, 0x8B2B,
											 0x1B6B, /*0xEE44, 0xD266, 0xA54E, 0xDDFB,*/ 0x6E7F, 0x3471, 0x03F5, 0x34B1,
											 0xF156, 0x561E, 0xF065, 0xAA69, 0x7AC3, 0x410D, 0x1A00, 0x91C1, 0xC375,
											 0xBB4D, 0xB0B0, 0x5D16};
	
	for (auto ind : faceIndices)
	{
		float randNum = ((float(rand() % 101) / 100.0f) * 10.0f) - 5.0f;
		invoke<Void>(0x5653AB26C82938CF, ped, ind, randNum);
	}
	
	invoke<Void>(0x5653AB26C82938CF, ped, 0xEE44, ((float(rand() % 101) / 100.0f) * 2.0f) + 1.0f);
	
	PED::_0xCC8CA3E88256E58F(ped, 0, 1, 1, 1, 0);
	
	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*) "WEAPON_REVOLVER_SCHOFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 200, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(ped, weaponHash, 200);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);
	
	
	PED::_0x4A48B6E03BABB4AC(ped, (Any*) name.c_str());
	
	//Ped pedCopy = ped;
	
	//ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&pedCopy);
	
	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 0.9f + (float(rand() % 11) / 100.0f));
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 200);
	ENTITY::SET_ENTITY_HEALTH(ped, 200, 0);
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
	
	
	MarkPedAsCompanion(ped);
	
	std::string str = "Spawned NPC with name " + name;
	
	ChaosMod::LogToFile(str.c_str());
}

void EffectEveryoneIsLenny::OnActivate()
{
	auto peds = GetNearbyPeds(70);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	for (auto ped : peds)
	{
		Hash model = ENTITY::GET_ENTITY_MODEL(ped);
		
		/** IS_MODEL_A_HORSE */
		bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, model);
		
		if (bModelIsHorse)
		{
			continue;
		}
		
		int relationships = PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, playerPed);
		
		if ((ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped) && !ChaosMod::pedsSet.contains(ped)) && relationships < 4)
		{
			continue;
		}
		
		Vector3 vec = ENTITY::GET_ENTITY_COORDS(ped, true, 0);
		float heading = ENTITY::GET_ENTITY_HEADING(ped);
		
		Vehicle veh = 0;
		Ped mount = 0;
		int32_t seatID = -2;
		
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, 0))
		{
			veh = PED::GET_VEHICLE_PED_IS_IN(ped, 0);
			
			Hash vehModel = ENTITY::GET_ENTITY_MODEL(veh);
			
			int32_t seatsNum = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(vehModel) - 1;
			
			for (int32_t i = -1; i < seatsNum; i++)
			{
				if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i) == ped)
				{
					seatID = i;
					break;
				}
			}
		}
		else if (PED::IS_PED_ON_MOUNT(ped))
		{
			mount = PED::GET_MOUNT(ped);
			
			if (PED::_0xB676EFDA03DADA52(mount, 0) == ped)
			{
				seatID = -1;
			}
			else
			{
				seatID = 0;
			}
		}
		
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, 0, 0);
		
		PED::DELETE_PED(&ped);
		
		static std::vector <Hash> skins = {GAMEPLAY::GET_HASH_KEY((char*) "CS_lenny"),
										   GAMEPLAY::GET_HASH_KEY((char*) "MSP_SALOON1_MALES_01"),
										   GAMEPLAY::GET_HASH_KEY((char*) "MSP_SALOON1_FEMALES_01")};
		
		int32_t skinID = rand() % skins.size();
		Ped lenny = SpawnPedAroundPlayer(skins[skinID]);
		int32_t outfitID = 0;
		
		switch (skinID)
		{
			case 0:
				outfitID = rand() % 13;
				break;
			case 1:
			{
				static std::vector <int32_t> outfits = {0, 2, 5, 7, 9, 11, 13, 15, 17, 21, 23, 25, 27, 29, 31, 33, 35,
														37, 39, 41, 43};
				
				outfitID = outfits[rand() % outfits.size()];
				
				break;
			}
			case 2:
			{
				static std::vector <int32_t> outfits = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
				
				outfitID = outfits[rand() % outfits.size()];
				break;
			}
		}
		
		invoke<Void>(0x77FF8D35EEC6BBC4, lenny, outfitID, false);
		
		ENTITY::SET_ENTITY_COORDS(lenny, vec.x, vec.y, vec.z, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_HEADING(lenny, heading);
		
		if (seatID != -2)
		{
			if (veh)
			{
				PED::SET_PED_INTO_VEHICLE(lenny, veh, seatID);
			}
			else if (mount && seatID == -1)
			{
				SetPedOnMount(lenny, mount, seatID);
				AI::TASK_WANDER_STANDARD(lenny, 10.0f, 10);
			}
		}
		else
		{
			AI::TASK_WANDER_STANDARD(lenny, 10.0f, 10);
			AI::TASK_LOOK_AT_ENTITY(ped, playerPed, -1, 2048, 3, 1);
		}
		
		if (relationships == 5)
		{
			MarkPedAsEnemy(lenny);
		}
		
		ChaosMod::pedsSet.erase(lenny);
		
		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&lenny);
	}
}

void EffectSpawnExtremeEvilMicah::OnActivate()
{
	Effect::OnActivate();
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*) "CS_MicahBell");
	
	Ped ped = SpawnPedAroundPlayer(pedModel, false, true);
	/** Set outfit */
	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 10, false);
	/** Set explosion proof */
	invoke<Void>(0xFAEE099C6F890BB8, ped, 4, 0);
	
	MarkPedAsEnemy(ped);
	
	RemoveAllPedWeapons(ped);
	
	static Hash weaponHash = GET_HASH("WEAPON_BOW");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	
	
	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 800);
	ENTITY::SET_ENTITY_HEALTH(ped, 800, 0);
	
	/** PCF_NoCriticalHits */
	PED::SET_PED_CONFIG_FLAG(ped, 263, true);
	
	lastVec.x = lastVec.y = lastVec.z = 0;
	micahPed = ped;
}

void EffectSpawnExtremeEvilMicah::OnTick()
{
	if (!ENTITY::DOES_ENTITY_EXIST(micahPed) || ENTITY::IS_ENTITY_DEAD(micahPed))
	{
		return;
	}
	
	Vector3 vec;
	
	bool bCoordValid = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(micahPed, &vec);
	
	if (!bCoordValid || (vec.x == lastVec.x && vec.y == lastVec.y && vec.z == lastVec.z))
	{
		return;
	}
	
	lastVec = vec;
	
	FIRE::ADD_EXPLOSION(vec.x, vec.y, vec.z, 27, 1.0f, true, false, 1.0f);
}
