#include "peds.h"

void LoadModel(Hash model)
{
	//if (STREAMING::IS_MODEL_VALID(model))
	//{
	//	return;
	//}

	STREAMING::REQUEST_MODEL(model, true);

	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
}

Ped SpawnPedAroundPlayer(Hash skinModel)
{
	LoadModel(skinModel);

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	Ped ped = PED::CREATE_PED(skinModel, playerLocation.x, playerLocation.y, playerLocation.z, 0.0f, 1, 0, 0, 0);
	PED::SET_PED_VISIBLE(ped, true);
	PED::SET_PED_HEARING_RANGE(ped, 10000.0f);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(skinModel);

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		PED::SET_PED_INTO_VEHICLE(ped, playerVehicle, -2);
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		Ped mount = PED::GET_MOUNT(playerPed);
		/** __SET_PED_ON_MOUNT */
		invoke<Void>(0x028F76B6E78246EB, ped, mount, -2, true);
	}

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
}

void MarkPedAsEnemy(Hash ped)
{
	Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");
	Hash enemyGroup;

	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_ENEMY", &enemyGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, enemyGroup);

	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, enemyGroup);

	/** BF_CanFightArmedPedsWhenNotArmed */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
	/** BF_AlwaysFight */
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

	AI::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 16);
}

void EffectSpawnSoldier::OnActivate() 
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"s_m_m_army_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);
}

void EffectSpawnDrunkardJon::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_GrizzledJon");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsEnemy(ped);
}

void EffectSpawnLenny::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"CS_lenny");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);

	static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"w_repeater_carbine01");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 9999, true, 0x2cd419dc);
	WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
}

void EffectSpawnChicken::OnActivate()
{
	Effect::OnActivate();

	static Hash skinModel = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Chicken_01");

	Ped ped = SpawnPedAroundPlayer(skinModel);

	MarkPedAsCompanion(ped);
}
