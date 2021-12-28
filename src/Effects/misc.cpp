#include "misc.h"
#include "vehs.h"
#include "peds.h"

void EffectSpawnHotchkissCannon::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"hotchkiss_cannon");

	LoadModel(model);

	VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, rand() % 360, false, false, false, false);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
}


void EffectTeleportEverything::OnActivate()
{
	std::vector<Entity> entities;
	auto nearbyPeds = GetNearbyPeds(45);
	auto nearbyVehs = GetNearbyVehs(10);

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			entities.push_back(ped);
		}
	}

	for (auto veh : nearbyVehs)
	{
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			entities.push_back(veh);
		}
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerCoord = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	for (auto entity : entities)
	{
		//ENTITY::SET_ENTITY_AS_MISSION_ENTITY(entity, true, true);
		ENTITY::SET_ENTITY_COORDS(entity, playerCoord.x, playerCoord.y, playerCoord.z, 0, 0, 0, 0);
	}

}

void EffectSnowstorm::OnActivate()
{
	Effect::OnActivate();

	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"WHITEOUT");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(50.0f);

	/** Set SNOW_LEVEL to -1 (enabled) */
	invoke<Void>(0xF6BEE7E80EC5CA40, -1);

	/** Set SNOW_COVERAGE_TYPE */
	invoke<Void>(0xF02A9C330BBFC5C7, 2);
}

void EffectSnowstorm::OnDeactivate()
{
	Effect::OnDeactivate();

	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"SUNNY");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(0.0f);

	/** Set SNOW_LEVEL to 1 (disabled) */
	invoke<Void>(0xF6BEE7E80EC5CA40, 1);

	/** Set SNOW_COVERAGE_TYPE */
	invoke<Void>(0xF02A9C330BBFC5C7, 0);
}

void EffectThunderstorm::OnActivate()
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"THUNDERSTORM");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(50.0f);
}

void EffectThunderstorm::OnDeactivate()
{
	Effect::OnDeactivate();

	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"SUNNY");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(0.0f);
}

void EffectGiantPeds::OnActivate()
{
	giantPeds.clear();
	SetScale(5.0f);
}

void EffectGiantPeds::OnDeactivate()
{
	for (auto ped : giantPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			/** _SET_PED_SCALE */
			invoke<Void>(0x25ACFC650B65C538, ped, 1.0f);
		}
	}

	giantPeds.clear();
}

void EffectGiantPeds::OnTick()
{
	if (GetTickCount() % 1000 == 0)
	{
		SetScale(5.0f);
	}
}

void EffectGiantPeds::SetScale(float scale)
{
	auto nearbyPeds = GetNearbyPeds(45);

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			/** _SET_PED_SCALE */
			invoke<Void>(0x25ACFC650B65C538, ped, scale);

			if (scale != 1.0f)
			{
				giantPeds.insert(ped);
			}
		}
	}
}

void EffectAllPedsWannaKillPlayer::OnActivate()
{
	Hash enemyGroup;
	PED::ADD_RELATIONSHIP_GROUP((char*)"_CHAOS_ENEMY_PEDS", &enemyGroup);
	static Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");

	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, enemyGroup);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	auto nearbyPeds = GetNearbyPeds(45);

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_REVOLVER_SCHOFIELD");
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(ped, weaponHash, 100);
			WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);

			PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, enemyGroup);

			/** BF_CanFightArmedPedsWhenNotArmed */
			PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
			/** BF_AlwaysFight */
			PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

			PED::SET_PED_FLEE_ATTRIBUTES(ped, 2, true);

			AI::TASK_COMBAT_PED(ped, playerPed, 0, 16);
		}
	}
}

void EffectAllPedsWannaKillPlayer::OnDeactivate()
{
	static Hash enemyGroup = GAMEPLAY::GET_HASH_KEY((char*)"_CHAOS_ENEMY_PEDS");
	static Hash playerGroup = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");

	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, enemyGroup, playerGroup);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, playerGroup, enemyGroup);
}

void EffectAllPedsWannaKillPlayer::OnTick()
{
	if (GetTickCount() % 1000)
	{
		return;
	}
	static Hash enemyGroup = GAMEPLAY::GET_HASH_KEY((char*)"_CHAOS_ENEMY_PEDS");

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	auto nearbyPeds = GetNearbyPeds(45);

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_REVOLVER_SCHOFIELD");
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(ped, weaponHash, 100);
			WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);

			/** BF_CanFightArmedPedsWhenNotArmed */
			PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
			/** BF_AlwaysFight */
			PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

			PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, enemyGroup);

			PED::SET_PED_FLEE_ATTRIBUTES(ped, 2, true);

			AI::TASK_COMBAT_PED(ped, playerPed, 0, 16);
		}
	}
}

void EffectRagdollEveryone::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(45);
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	nearbyPeds.push_back(playerPed);

	for (Ped ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 0, true, true, false);
		}
	}
}

void EffectLaunchPedsUp::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(45);
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	bool bPlayerOnMount = PED::IS_PED_ON_MOUNT(playerPed);
	Ped playerMount;

	if (bPlayerOnMount)
	{
		playerMount = PED::GET_MOUNT(playerPed);
	}

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped) && (!bPlayerOnMount || playerMount != ped))
		{
			PED::SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);
			Vector3 entityVelocity = ENTITY::GET_ENTITY_VELOCITY(ped, 0);

			ENTITY::SET_ENTITY_VELOCITY(ped, entityVelocity.x, entityVelocity.y, 35.0f);
		}
	}

}

void EffectInvertedGravity::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(45);
	entities.clear();

	nearbyPeds.push_back(PLAYER::PLAYER_PED_ID());

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			PED::SET_PED_TO_RAGDOLL(ped, 20000, 20000, 0, true, true, false);
		}
	}
}

void EffectInvertedGravity::OnTick()
{
	Effect::OnTick();

	if (GetTickCount() % 1000 == 0)
	{
		entities.clear();
		auto nearbyPeds = GetNearbyPeds(45);
		auto nearbyVehs = GetNearbyVehs(45);

		for (auto ped : nearbyPeds)
		{
			entities.insert(ped);
		}

		for (auto veh : nearbyVehs)
		{
			entities.insert(veh);
		}

		entities.insert(PLAYER::PLAYER_PED_ID());
	}

	for (auto entity : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(entity, 0, 0.0f, 0.0f, 25.0f, false, false, true, false);
		}
	}
}

# define M_PI	3.14159265358979323846

void EffectLightnings::OnActivate()
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"THUNDERSTORM");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(150.0f);
}

void EffectLightnings::OnDeactivate()
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"SUNNY");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(0.0f);
}

void EffectLightnings::OnTick()
{
	if (GetTickCount() % 1000)
	{
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerVec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	float radius = float(rand() % 100);

	/** In radians */
	float angle = float(rand() % 360) * (M_PI / 180.0f);

	playerVec.x += radius * sin(angle);
	playerVec.y += radius * cos(angle);

	invoke<Void>(0x67943537D179597C, playerVec.x, playerVec.y, playerVec.z);
}
