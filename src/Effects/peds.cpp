#include "peds.h"
#include "../script.h"
#include "misc.h"

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
	DECORATOR::DECOR_SET_INT(ped, (char*)"honor_override", 0);
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
			static Hash mountModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Horse_Morgan_Bay");

			Ped mount = SpawnPedAroundPlayer(mountModel, false);
			SetPedOnMount(ped, mount, -1);
		}
	}

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, false, false);

	return ped;
}

void MarkPedAsCompanion(Hash ped)
{
	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");
	Hash companionGroup;

	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_COMPANION", &companionGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, companionGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, playerGroup, companionGroup);

	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, companionGroup);

	PED::SET_PED_AS_GROUP_MEMBER(ped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID()));

	/** BF_CanFightArmedPedsWhenNotArmed */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	/** BF_AlwaysFight */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);
}

void MarkPedAsEnemy(Hash ped)
{
	static std::vector<Hash> groups = {
		GAMEPLAY::GET_HASH_KEY((char*)"PLAYER"),
		0x8A33CDCF, // Civ Male
		0x3220F762 // Civ Female
	};

	Hash enemyGroup;
	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_ENEMY", &enemyGroup);

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

void EffectSpawnSoldier::OnActivate() 
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"s_m_m_army_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);

	RemoveAllPedWeapons(ped);

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_SNIPERRIFLE_ROLLINGBLOCK");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
}

void EffectSpawnDrunkardJon::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_GrizzledJon");

	Ped ped = SpawnPedAroundPlayer(skinModel, false, true);

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);

	MarkPedAsEnemy(ped);
}

void EffectSpawnLenny::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_lenny");
	static Hash salonModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_lenny");

	static std::vector<Hash> skins = {
		GAMEPLAY::GET_HASH_KEY((char*)"CS_lenny"),
		GAMEPLAY::GET_HASH_KEY((char*)"MSP_SALOON1_MALES_01"),
		GAMEPLAY::GET_HASH_KEY((char*)"MSP_SALOON1_FEMALES_01")
	};

	int32_t skinID = rand() % skins.size();

	Ped ped = SpawnPedAroundPlayer(skins[skinID]);

	MarkPedAsCompanion(ped);

	RemoveAllPedWeapons(ped);

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_RIFLE_SPRINGFIELD");
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
			static std::vector<int32_t> outfits = {
				0, 2, 5, 7, 9, 11, 13, 15, 17, 21, 23,
				25, 27, 29, 31, 33, 35, 37, 39, 41, 43
			};

			outfitID = outfits[rand() % outfits.size()];

			break;
		}
	case 2:
		{
		static std::vector<int32_t> outfits = {
			2, 4, 6, 8, 10, 12, 14, 16, 18, 20
		};

		outfitID = outfits[rand() % outfits.size()];
			break;
		}
	}

	invoke<Void>(0x77FF8D35EEC6BBC4, ped, outfitID, false);

}

void EffectSpawnChicken::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Chicken_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);
}

void EffectKidnapping::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_AberdeenPigFarmer");
	static Hash skinModel2 = GAMEPLAY::GET_HASH_KEY((char*)"CS_AberdeenSister");

	Ped ped = SpawnPedAroundPlayer(skinModel);
	Ped sister = SpawnPedAroundPlayer(skinModel2);

	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");
	Hash relationshipGroup;

	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_KIDNAPPERS", &relationshipGroup);
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

	static Hash wagonModel = GAMEPLAY::GET_HASH_KEY((char*)"WAGONPRISON01X");
	LoadModel(wagonModel);

	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	vehicle = VEHICLE::CREATE_VEHICLE(wagonModel, playerCoord.x, playerCoord.y, playerCoord.z, playerHeading, 0, 0, 0, 0);
	DECORATOR::DECOR_SET_BOOL(vehicle, (char*)"wagon_block_honor", true);
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
		AI::TASK_VEHICLE_DRIVE_WANDER(ped, vehicle, 100000.0f, 262144);
		PED::SET_PED_KEEP_TASK(ped, true);
	}
}

void EffectSpawnHorse::OnActivate()
{
	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Horse_Arabian_White");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	SetPedOnMount(playerPed, horse, -1);
}


std::vector<Ped> GetNearbyPeds(int32_t Max)
{
	std::vector<Ped> pedsOut;
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	NearbyEntities peds;
	peds.size = 100;

	if (Max > 100)
	{
		Max = 100;
	}

	int pedsFound = PED::GET_PED_NEARBY_PEDS(playerPed, (int*)&peds, -1, 0);

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

void RemoveAllPedWeapons(Ped ped)
{
	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_UNARMED");
	WEAPON::SET_CURRENT_PED_WEAPON(ped, unarmed, 1, 0, 0, 0);

	for (Hash weaponHash : Effect::WeaponHashes)
	{
		WEAPON::REMOVE_WEAPON_FROM_PED(ped, weaponHash, 0, 0);
	}
}

void EffectSpawnMule::OnActivate()
{
	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_HorseMule_01");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnDonkey::OnActivate()
{
	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Donkey_01");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnSerialKiller::OnActivate()
{
	/** Spawn donkey */

	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Donkey_01");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** Spawn killer */
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*)"G_M_M_UniDuster_03");

	Ped ped = SpawnPedAroundPlayer(pedModel);

	SetPedOnMount(ped, horse, -1);

	MarkPedAsEnemy(ped);
}

void EffectSpawnVampire::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_Vampire");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 1000);
	ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);


	MarkPedAsEnemy(ped);
}

void EffectSpawnGiantDonkey::OnActivate()
{
	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Donkey_01");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, horse, 1.5f);

	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnMiniDonkey::OnActivate()
{
	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Donkey_01");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, horse, 0.42f);

	SetPedOnMount(playerPed, horse, -1);
}

void EffectSpawnGiantCop::OnActivate()
{
	Effect::OnActivate();

	static Hash copGroup = GAMEPLAY::GET_HASH_KEY((char*)"COP");
	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"S_M_M_DispatchLeaderPolice_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 10.0f);

	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, copGroup);

	AI::TASK_LOOK_AT_ENTITY(ped, PLAYER::PLAYER_PED_ID(), -1, 2048, 3, 1);
}

void EffectSpawnAngrySkeleton::OnActivate()
{
	Effect::OnActivate();

	static std::vector<const char*> skins = {
		"U_M_M_CircusWagon_01",
		//"CS_ODPROSTITUTE",
		"A_M_M_UniCorpse_01",
		"A_F_M_UniCorpse_01"
	};

	uint32_t skinID = rand() % skins.size();

	Ped ped = SpawnPedAroundPlayer(GAMEPLAY::GET_HASH_KEY((char*)skins[skinID]));

	if (skinID == 1)
	{
		static std::vector<uint32_t> skeletonOutfitsIDs = {
			31, 36, 46, 56, 68, 143, 144, 147, 
		};

		/** Set outfit */
		invoke<Void>(0x77FF8D35EEC6BBC4, ped, skeletonOutfitsIDs[rand() % skeletonOutfitsIDs.size()], false);
	}
	else if (skinID == 2)
	{
		static std::vector<uint32_t> skeletonOutfitsIDs = {
			11, 12, 27, 28, 
		};

		/** Set outfit */
		invoke<Void>(0x77FF8D35EEC6BBC4, ped, skeletonOutfitsIDs[rand() % skeletonOutfitsIDs.size()], false);
	}

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);

	if (rand() % 2 == 0)
	{
		static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_MELEE_BROKEN_SWORD");
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 1, true, 0x2cd419dc);
		WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
	}

	MarkPedAsEnemy(ped);
}

void EffectSpawnAngryDwarf::OnActivate()
{
	Effect::OnActivate();

	/** Spawn donkey */

	Effect::OnActivate();

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Donkey_01");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, horse, 0.5f);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** Spawn dwarf */
	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_Magnifico");

	Ped ped = SpawnPedAroundPlayer(pedModel, false);

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_MELEE_KNIFE");
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

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_ODDFELLOWSPINHEAD");

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

	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*)"RE_FROZENTODEATH_FEMALES_01");
	static Hash pedModel2 = GAMEPLAY::GET_HASH_KEY((char*)"RE_FROZENTODEATH_MALES_01");

	Ped ped = SpawnPedAroundPlayer(pedModel);
	Ped ped2 = SpawnPedAroundPlayer(pedModel2);

	MarkPedAsEnemy(ped);
	MarkPedAsEnemy(ped2);
}

void EffectSpawnRobot::OnActivate()
{
	Effect::OnActivate();

	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_crackpotRobot");

	Ped ped = SpawnPedAroundPlayer(pedModel);

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 10000);
	ENTITY::SET_ENTITY_HEALTH(ped, 10000, 0);
	
	MarkPedAsCompanion(ped);
}

void EffectSpawnLassoGuy::OnActivate()
{
	Effect::OnActivate();

	static Hash pedModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_EXOTICCOLLECTOR");

	Ped ped = SpawnPedAroundPlayer(pedModel, false, true);

	MarkPedAsEnemy(ped);

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_LASSO");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	/** TASK_LASSO_PED */
	invoke<Void>(0xC716EB2BD16370A3, ped, PLAYER::PLAYER_PED_ID());
}

void EffectSkyrimIntro::OnActivate()
{
	Effect::OnActivate();

	GRAPHICS::ANIMPOSTFX_PLAY((char*)"PlayerWakeUpInterrogation");

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerCoord = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash copModel = GAMEPLAY::GET_HASH_KEY((char*)"U_M_O_BlWPoliceChief_01");
	static Hash wagonModel = GAMEPLAY::GET_HASH_KEY((char*)"wagon03x");
	static Hash prisonerModel1 = GAMEPLAY::GET_HASH_KEY((char*)"RE_PRISONWAGON_MALES_01");
	static Hash prisonerModel2 = GAMEPLAY::GET_HASH_KEY((char*)"RE_LONEPRISONER_MALES_01");
	static Hash prisonerModel3 = GAMEPLAY::GET_HASH_KEY((char*)"CS_chainprisoner_01");

	Ped cop = SpawnPedAroundPlayer(copModel, false);
	Ped prisoner1 = SpawnPedAroundPlayer(prisonerModel1, false);
	Ped prisoner2 = SpawnPedAroundPlayer(prisonerModel2, false);
	Ped prisoner3 = SpawnPedAroundPlayer(prisonerModel3, false);

	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");
	Hash relationshipGroup;

	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_WAGON", &relationshipGroup);
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

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_REPEATER_WINCHESTER");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(cop, weaponHash, 100, true, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(playerPed, weaponHash, 100);

	LoadModel(wagonModel);

	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(wagonModel, playerCoord.x, playerCoord.y, playerCoord.z, playerHeading, 0, 0, 0, 0);

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

	AI::TASK_VEHICLE_DRIVE_WANDER(cop, vehicle, 10000.0f, 411);
	PED::SET_PED_KEEP_TASK(cop, true);

	PlayAmbientSpeech("0132_G_M_M_UNICRIMINALS_01_BLACK_01", "CALLOUT_CAMP_WAKE_UP", playerPed, 0, false);
}

void EffectSpawnParrotCompanion::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Parrot_01");

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

	static Hash horseModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Horse_Shire_DarkBay");

	Ped horse = SpawnPedAroundPlayer(horseModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	SetPedOnMount(playerPed, horse, -1);
}

void EffectUndeadNightmare::OnActivate()
{
	Effect::OnActivate();

	TIME::SET_CLOCK_TIME(22, 0, 0);

	static Hash weatherHash = GAMEPLAY::GET_HASH_KEY((char*)"FOG");

	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	GAMEPLAY::SET_WEATHER_TYPE(weatherHash, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	static Hash skin = GAMEPLAY::GET_HASH_KEY((char*)"A_M_M_ARMCHOLERACORPSE_01");
	static Hash skin2 = GAMEPLAY::GET_HASH_KEY((char*)"A_M_M_UniCorpse_01");

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 playerVec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash copGroup = GAMEPLAY::GET_HASH_KEY((char*)"COP");
	static Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");


	Hash enemyGroup;

	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_ZOMBIE", &enemyGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, enemyGroup);

	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, copGroup, enemyGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, copGroup);

	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, 2318650831, enemyGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, 2318650831);

	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, 841021282, enemyGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, 841021282);

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

		static Hash combatMod = GAMEPLAY::GET_HASH_KEY((char*)"MeleeApproach");
		
		/** _SET_PED_COMBAT_STYLE_MOD */
		invoke<Void>(0x8B1E8E35A6E814EA, zombie, combatMod, -1.0f);

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(zombie, enemyGroup);

		/** BF_CanFightArmedPedsWhenNotArmed */
		PED::SET_PED_COMBAT_ATTRIBUTES(zombie, 5, true);
		/** BF_AlwaysFight */
		PED::SET_PED_COMBAT_ATTRIBUTES(zombie, 46, true);
		/** BF_CanUseVehicles */
		PED::SET_PED_COMBAT_ATTRIBUTES(zombie, 1, false);
		/** BF_CanLeaveVehicle */
		PED::SET_PED_COMBAT_ATTRIBUTES(zombie, 3, true);

		AI::TASK_COMBAT_PED(zombie, PLAYER::PLAYER_PED_ID(), 0, 16);
	}

	PlayAmbientSpeech("RCMP", "RCMP_DEAD", playerPed, 0, false);

}

void EffectSpawnDogCompanion::OnActivate()
{
	Effect::OnActivate();

	static std::vector<const char*> dogModels = {
		"A_C_DogAmericanFoxhound_01",
		"A_C_DogAustralianSheperd_01",
		"A_C_DogBluetickCoonhound_01",
		"A_C_DogCatahoulaCur_01",
		"A_C_DogChesBayRetriever_01",
		"A_C_DogCollie_01",
		"A_C_DogHobo_01",
		"A_C_DogHound_01",
		"A_C_DogHusky_01"
	};

	Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)dogModels[rand() % dogModels.size()]);

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);
}

void EffectSpawnCatCompanion::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Cat_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);
}

void EffectSpawnBearCompanion::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_BearBlack_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);
}

void EffectSpawnAngryCorpse::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_M_M_ARMCHOLERACORPSE_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 300);
	ENTITY::SET_ENTITY_HEALTH(ped, 300, 0);

	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 13, false);

	MarkPedAsEnemy(ped);
}

void EffectSpawnAngryCaveman::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_M_M_UniCorpse_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);

	invoke<Void>(0x77FF8D35EEC6BBC4, ped, 44, false);

	MarkPedAsEnemy(ped);

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, ped, 1.1f);
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

	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_UNARMED");
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

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"S_M_M_ValCowpoke_01");

	Ped ped = SpawnPedAroundPlayer(skinModel, false, true);

	ENTITY::SET_ENTITY_MAX_HEALTH(ped, 500);
	ENTITY::SET_ENTITY_HEALTH(ped, 500, 0);

	invoke<Void>(0x77FF8D35EEC6BBC4, ped, rand() % 22, false);

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_REVOLVER_SCHOFIELD");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 200, 1, 0x2cd419dc);
	WEAPON::SET_PED_AMMO(ped, weaponHash, 200);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	MarkPedAsEnemy(ped);

}
