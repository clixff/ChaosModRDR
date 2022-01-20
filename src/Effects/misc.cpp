#include "misc.h"
#include "vehs.h"
#include "peds.h"
#include "../script.h"


Entity SpawnObject(Hash model)
{
	LoadModel(model);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 vec = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	Entity object = OBJECT::CREATE_OBJECT(model, vec.x, vec.y, vec.z, 1, 1, 0, 0, 1);

	ENTITY::SET_ENTITY_VISIBLE(object, true);
	ENTITY::SET_ENTITY_ALPHA(object, 255, true);

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(object, false, false);
	Entity objCopy = object;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&objCopy);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	if (ENTITY::DOES_ENTITY_EXIST(object))
	{
		ChaosMod::propsSet.insert(object);
	}

	return object;
}

float GetDistance3D(Vector3 a, Vector3 b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}


Vector3 GetRandomCoordInRange(Vector3 vec, float distance)
{
	/** In radians */
	float angle = float(rand() % 360) * (M_PI / 180.0f);

	vec.x += distance * sin(angle);
	vec.y += distance * cos(angle);

	return vec;
}

Vector3 GetRandomCoordAroundPlayer(float distance, bool bUseVelocity)
{
	Entity entity = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(entity, false))
	{
		entity = PED::GET_VEHICLE_PED_IS_IN(entity, false);
	}
	else if (PED::IS_PED_ON_MOUNT(entity))
	{
		entity = PED::GET_MOUNT(entity);
	}

	Vector3 vec = ENTITY::GET_ENTITY_COORDS(entity, true, 0);

	if (bUseVelocity)
	{
		Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(entity, 0);

		vec.x += velocity.x * 2.0f;
		vec.y += velocity.y * 2.0f;
		vec.z += velocity.z * 2.0f;
	}

	return GetRandomCoordInRange(vec, distance);
}

void EffectSpawnHotchkissCannon::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"hotchkiss_cannon");

	LoadModel(model);

	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);

	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, playerHeading, false, false, false, false);

	Vehicle vehCopy = veh;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && !PED::IS_PED_ON_MOUNT(playerPed))
	{
		PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
	}
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

		if (ENTITY::IS_ENTITY_A_PED(entity))
		{
			PED::SET_PED_TO_RAGDOLL(entity, 3000, 3000, 0, true, true, false);
		}
	}

	props.clear();
	oldPropsCoords.clear();

	int* objects = new int[10];

	int32_t foundObjects = worldGetAllObjects(objects, 10);

	for (int32_t i = 0; i < foundObjects; i++)
	{
		Entity entity = objects[i];
		
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			props.push_back(entity);
			oldPropsCoords.push_back(ENTITY::GET_ENTITY_COORDS(entity, false, 0));
			ENTITY::SET_ENTITY_DYNAMIC(entity, true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(entity, true);
			ENTITY::SET_ENTITY_COORDS(entity, playerCoord.x, playerCoord.y, playerCoord.z, 0, 0, 0, 0);
		}
	}

	delete[] objects;
}

void EffectTeleportEverything::OnDeactivate()
{
	for (int32_t i = 0; i < props.size(); i++)
	{
		Entity entity = props[i];

		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			Vector3 oldCoord = oldPropsCoords[i];
			ENTITY::SET_ENTITY_DYNAMIC(entity, false);
			ENTITY::SET_ENTITY_HAS_GRAVITY(entity, false);
			ENTITY::SET_ENTITY_COORDS(entity, oldCoord.x, oldCoord.y, oldCoord.z, 0, 0, 0, 0);
		}
	}

	props.clear();
	oldPropsCoords.clear();
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
	if (TimerTick(1000))
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
	if (!TimerTick(1000))
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
			//static Hash weaponHash = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_REVOLVER_SCHOFIELD");
			//WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, 1, 0x2cd419dc);
			//WEAPON::SET_PED_AMMO(ped, weaponHash, 100);
			//WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, 1, 0, 0, 0);

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
	auto nearbyPeds = GetNearbyPeds(100);
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
	auto nearbyPeds = GetNearbyPeds(100);
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
	auto nearbyPeds = GetNearbyPeds(100);
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

	if (TimerTick(1000))
	{
		entities.clear();
		auto nearbyPeds = GetNearbyPeds(45);
		auto nearbyVehs = GetNearbyVehs(45);
		auto nearbyProps = GetNearbyProps(45);

		for (auto ped : nearbyPeds)
		{
			entities.insert(ped);
		}

		for (auto veh : nearbyVehs)
		{
			entities.insert(veh);
		}

		for (auto prop : nearbyProps)
		{
			ENTITY::SET_ENTITY_DYNAMIC(prop, true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(prop, true);
			entities.insert(prop);
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


void EffectDoomsday::OnActivate()
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"THUNDERSTORM");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(150.0f);
	GRAPHICS::SET_TIMECYCLE_MODIFIER((char*)"EagleEyeTest");
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectDoomsday::OnDeactivate()
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	static Hash weather = GAMEPLAY::GET_HASH_KEY((char*)"SUNNY");
	GAMEPLAY::SET_WEATHER_TYPE(weather, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	GAMEPLAY::SET_WIND_SPEED(0.0f);

	GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectDoomsday::OnTick()
{
	if (TimerTick(250))
	{
		entities.clear();
		auto nearbyPeds = GetNearbyPeds(45);
		auto nearbyVehs = GetNearbyVehs(45);
		auto nearbyProps = GetNearbyProps(20);

		Ped playerPed = PLAYER::PLAYER_PED_ID();

		nearbyPeds.push_back(playerPed);

		for (auto ped : nearbyPeds)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				PED::SET_PED_TO_RAGDOLL(ped, 1000, 1000, 0, true, true, false);
				entities.insert(ped);
			}
		}

		for (auto veh : nearbyVehs)
		{
			entities.insert(veh);
		}

		for (auto prop : nearbyProps)
		{
			ENTITY::SET_ENTITY_DYNAMIC(prop, true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(prop, true);
			entities.insert(prop);
		}


		Vector3 vec = GetRandomCoordAroundPlayer(rand() % 100);

		/** _FORCE_LIGHTNING_FLASH_AT_COORDS */
		invoke<Void>(0x67943537D179597C, vec.x, vec.y, vec.z);

		randomDirection.x = float((rand() % 5) + 1) * (rand() % 2 ? -1.0f : 1.0f);
		randomDirection.y = float((rand() % 5) + 1) * (rand() % 2 ? -1.0f : 1.0f);
		randomDirection.z = float((rand() % 5) + 1) * (rand() % 2 ? -1.0f : 1.0f);

	}

	for (auto entity : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			Vector3 direction = randomDirection;

			direction.x *= float((rand() % 51) + 50) / 100.0f;
			direction.y *= float((rand() % 51) + 50) / 100.0f;
			direction.z *= float((rand() % 51) + 50) / 100.0f;

			//ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(entity, 0, randomDirection.x, randomDirection.y, randomDirection.z, false, false, true, false);
			ENTITY::SET_ENTITY_VELOCITY(entity, direction.x, direction.y, direction.z);
		}
	}
}

void EffectPlayIntro::OnActivate()
{
	GRAPHICS::ANIMPOSTFX_PLAY((char*)"Title_GameIntro");
}

void EffectPlayIntro::OnDeactivate()
{
	GRAPHICS::ANIMPOSTFX_STOP((char*)"Title_GameIntro");
}

void SetWeather(Hash hash)
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	GAMEPLAY::SET_WEATHER_TYPE(hash, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
}

void EffectSetRandomWeather::OnActivate()
{
	Hash weatherHash = Effect::weatherHashes[rand() % Effect::weatherHashes.size()];

	SetWeather(weatherHash);
}

void EffectSetTimeMorning::OnActivate()
{
	TIME::SET_CLOCK_TIME(7, 0, 0);
}

void EffectSetTimeNight::OnActivate()
{
	TIME::SET_CLOCK_TIME(22, 0, 0);
}

void EffectSetRandomTime::OnActivate()
{
	TIME::SET_CLOCK_TIME(rand() % 24, 0, 0);
}

void EffectSetSunnyWeather::OnActivate()
{
	SetWeather(GAMEPLAY::GET_HASH_KEY((char*)"SUNNY"));
}

void EffectSetRainyWeather::OnActivate()
{
	SetWeather(GAMEPLAY::GET_HASH_KEY((char*)"RAIN"));
}

void EffectSetRapidWeather::OnDeactivate()
{
	SetWeather(GAMEPLAY::GET_HASH_KEY((char*)"SUNNY"));
}

void EffectSetRapidWeather::OnTick()
{
	if (TimerTick(500))
	{
		Hash weatherHash = Effect::weatherHashes[rand() % Effect::weatherHashes.size()];

		SetWeather(weatherHash);
	}
}

void EffectEarthquake::OnTick()
{
	if (TimerTick(500))
	{
		entities.clear();
		auto nearbyPeds = GetNearbyPeds(45);
		auto nearbyVehs = GetNearbyVehs(45);
		auto nearbyProps = GetNearbyProps(20);

		Ped playerPed = PLAYER::PLAYER_PED_ID();

		nearbyPeds.push_back(playerPed);

		for (auto ped : nearbyPeds)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				PED::SET_PED_TO_RAGDOLL(ped, 1000, 1000, 0, true, true, false);
				entities.insert(ped);
			}
		}

		for (auto veh : nearbyVehs)
		{
			entities.insert(veh);
		}

		for (auto prop : nearbyProps)
		{
			ENTITY::SET_ENTITY_DYNAMIC(prop, true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(prop, true);
			entities.insert(prop);
		}
	}

	for (auto entity : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			Vector3 randomDirection;
			randomDirection.x = float((rand() % 5) + 1);
			randomDirection.y = float((rand() % 5) + 1);
			randomDirection.z = float((rand() % 7) + 7);

			randomDirection.x *= rand() % 2 ? -1.0f : 1.0f;
			randomDirection.y *= rand() % 2 ? -1.0f : 1.0f;
			randomDirection.z *= rand() % 2 ? -1.0f : 1.0f;

			ENTITY::SET_ENTITY_VELOCITY(entity, randomDirection.x, randomDirection.y, randomDirection.z);
		}
	}
}

void EffectEveryoneIsInvincible::OnDeactivate()
{
	for (auto ped : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
		}
	}

	entities.clear();
}

void EffectEveryoneIsInvincible::OnTick()
{
	if (!TimerTick(1000))
	{
		return;
	}

	auto peds = GetNearbyPeds(45);

	peds.push_back(PLAYER::PLAYER_PED_ID());

	for (auto ped : peds)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(ped, true);

		entities.insert(ped);
	}
}

void Effect120FOV::OnActivate()
{
	this->cam = CAM::CREATE_CAM((char*)"DEFAULT_SCRIPTED_CAMERA", 1);
	CAM::RENDER_SCRIPT_CAMS(true, true, 500, 1, 1, 1);
}

void Effect120FOV::OnDeactivate()
{
	CAM::SET_CAM_ACTIVE(this->cam, false);
	CAM::RENDER_SCRIPT_CAMS(false, true, 700, 1, 1, 1);
	CAM::DESTROY_CAM(this->cam, true);
	this->cam = 0;
}

void Effect120FOV::OnTick()
{
	CAM::SET_CAM_ACTIVE(this->cam, true);

	Vector3 camCoord = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 camRotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
	CAM::SET_CAM_PARAMS(this->cam, camCoord.x, camCoord.y, camCoord.z, camRotation.x, camRotation.y, camRotation.z, 120, 0, 0, 2, 0, 0, 0);
}

void EffectIgniteNearbyPeds::OnActivate()
{
	auto peds = GetNearbyPeds(45);

	for (auto ped : peds)
	{
		Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

		/** IS_MODEL_A_HORSE */
		bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);

		/** Don't ignite horses */
		if (!bModelIsHorse)
		{
			FIRE::START_ENTITY_FIRE(ped, 1.0f, 0, 0);
		}
	}
}

void EffectLightningOnce::OnActivate()
{
	Vector3 vec = GetRandomCoordAroundPlayer(float((rand() % 5) + 2), false);

	/** _FORCE_LIGHTNING_FLASH_AT_COORDS */
	invoke<Void>(0x67943537D179597C, vec.x, vec.y, vec.z);
}

std::vector<Entity> GetNearbyProps(int32_t Max)
{
	std::vector<Entity> propsOut;

	if (Max > 255)
	{
		Max = 255;
	}

	int* worldProps = new int[255];

	int found = worldGetAllObjects(worldProps, 255);

	if (found < Max)
	{
		Max = found;
	}

	for (int32_t i = 0; i < Max; i++)
	{
		Entity prop = worldProps[i];
		if (ENTITY::DOES_ENTITY_EXIST(prop))
		{
			propsOut.push_back(prop);
		}
	}

	delete[] worldProps;

	return propsOut;
}

void PlayAmbientSpeech(const char* voiceDict, const char* voiceSpeech, Ped ped, uint32_t speechID, bool bSetEntity)
{
	struct
	{
		const char* speechName = "";
		const char* voiceName = "";
		alignas(8) int v3 = 0;
		alignas(8) Hash speechParamHash = GAMEPLAY::GET_HASH_KEY((char*)"speech_params_force");
		alignas(8) Entity entity;
		alignas(8) BOOL v6 = true;
		alignas(8) int v7 = 1;
		alignas(8) int v8 = 1;
	} speechData;

	speechData.v3 = speechID;

	speechData.speechName = voiceSpeech;
	speechData.voiceName = voiceDict;

	if (bSetEntity)
	{
		speechData.entity = ped;
	}

	AUDIO::_PLAY_AMBIENT_SPEECH1(ped, (char*)&speechData);
}

void EffectLightningEnemy::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	auto nearbyPeds = GetNearbyPeds(45);

	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			int rel = PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, playerPed);

			/** If ped is an enemy */
			if (rel == 5)
			{
				Vector3 vec = ENTITY::GET_ENTITY_COORDS(ped, true, 0);

				ENTITY::SET_ENTITY_HEALTH(ped, 1, 0);

				/** _FORCE_LIGHTNING_FLASH_AT_COORDS */
				invoke<Void>(0x67943537D179597C, vec.x, vec.y, vec.z);

				return;
			}
		}
	}
}

void EffectAltTab::OnActivate()
{
	INPUT inputs[4] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_MENU;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_TAB;

	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = VK_TAB;
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_MENU;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

void EffectRemoveWeaponFromEveryone::OnActivate()
{
	Effect::OnActivate();

	auto peds = GetNearbyPeds(45);

	peds.push_back(PLAYER::PLAYER_PED_ID());

	for (auto ped : peds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

			/** IS_MODEL_A_HORSE */
			bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);

			if (!bModelIsHorse)
			{
				RemoveAllPedWeapons(ped);
			}
		}
	}
}

void EffectSetFoggyWeather::OnActivate()
{
	SetWeather(GAMEPLAY::GET_HASH_KEY((char*)"FOG"));
}



void EffectGhostTown::OnActivate()
{
	Effect::OnActivate();

	entities.clear();
}

void EffectGhostTown::OnDeactivate()
{
	Effect::OnDeactivate();

	entities.insert(PLAYER::PLAYER_PED_ID());

	for (auto entity : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			ENTITY::SET_ENTITY_VISIBLE(entity, true);
		}
	}

	entities.clear();
}

void EffectGhostTown::OnTick()
{
	Effect::OnTick();

	if (!TimerTick(1000))
	{
		return;
	}

	auto nearbyPeds = GetNearbyPeds(40);
	auto nearbyVehs = GetNearbyVehs(40);

	auto nearbyEntites = nearbyVehs;

	for (auto ped : nearbyPeds)
	{
		nearbyEntites.push_back(ped);
	}

	for (auto entity : nearbyEntites)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			entities.insert(entity);
			ENTITY::SET_ENTITY_VISIBLE(entity, false);
		}
	}
}

void EffectSpawnUFO::OnActivate()
{
	TIME::SET_CLOCK_TIME(2, 0, 0);

	static Hash weatherHash = GAMEPLAY::GET_HASH_KEY((char*)"FOG");

	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	GAMEPLAY::SET_WEATHER_TYPE(weatherHash, 0, 1, 0, 0.0, 0);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();

	movesCount = 0;
	this->ufo = SpawnObject(0xC92962E3);
	ENTITY::SET_ENTITY_DYNAMIC(ufo, true);
	ENTITY::SET_ENTITY_HAS_GRAVITY(ufo, true);
	heading = 0.0f;

	Vector3 vec = ENTITY::GET_ENTITY_COORDS(ufo, true, 1);

	vec.z += 2.0f;

	ENTITY::SET_ENTITY_COORDS(ufo, vec.x, vec.y, vec.z, false, false, false, false);

	static Hash blipHash = GET_HASH("BLIP_STYLE_FRIENDLY");

	/** BLIP_ADD_FOR_ENTITY */
	Blip blip = RADAR::_0x23F74C2FDA6E7C61(blipHash, ufo);


	GRAPHICS::SET_TIMECYCLE_MODIFIER((char*)"PLayerSpottedDark");
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);

	AUDIO::_0x6FB1DA3CA9DA7D90((Any*)"Loop_A", ufo, (Any*)"Ufos_Sounds", 0, 0, 0);
}

void EffectSpawnUFO::OnTick()
{
	if (!ENTITY::DOES_ENTITY_EXIST(ufo))
	{
		return;
	}

	ENTITY::SET_ENTITY_ROTATION(ufo, 0.0f, 0.0f, 0.0f, 2, 0);

	heading += ChaosMod::GetDeltaTimeSeconds() * 2000.0f;

	ENTITY::SET_ENTITY_HEADING(ufo, heading);

	if (TimerTick(500) && ENTITY::DOES_ENTITY_EXIST(ufo))
	{
		Vector3 vel;
		vel.x = 175.0f;
		vel.y = 175.0f;
		vel.z = 5.0f;

		vel.x *= rand() % 2 ? 1.0f : -1.0f;
		vel.y *= rand() % 2 ? 1.0f : -1.0f;
		vel.z *= rand() % 2 ? 1.0f : -1.0f;

		if (movesCount % 2)
		{
			auto nearbyPeds = GetNearbyPeds(50);
			nearbyPeds.push_back(PLAYER::PLAYER_PED_ID());
			std::vector<Ped> validPeds;

			for (auto ped : nearbyPeds)
			{
				if (ENTITY::DOES_ENTITY_EXIST(ped) && PED::IS_PED_HUMAN(ped))
				{
					validPeds.push_back(ped);
				}
			}

			if (validPeds.size())
			{
				Ped ped = validPeds[rand() % validPeds.size()];

				Vector3 pedCoord = ENTITY::GET_ENTITY_COORDS(ped, true, 1);
				Vector3 ufoCoord = ENTITY::GET_ENTITY_COORDS(ufo, true, 1);

				Vector3 diff = pedCoord;
				diff.x -= ufoCoord.x;
				diff.y -= ufoCoord.y;
				diff.z -= ufoCoord.z;

				const float squareSum = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
				const float length = sqrt(squareSum);
				diff.x /= length;
				diff.y /= length;
				diff.z /= length;

				const float speed = 100.0f;

				diff.x *= speed;
				diff.y *= speed;
				diff.z *= speed;

				vel = diff;
			}
		}

		movesCount++;
		ENTITY::SET_ENTITY_VELOCITY(ufo, vel.x, vel.y, vel.z);

	}
}

void EffectSpawnUFO::OnDeactivate()
{
	Vector3 vec = ENTITY::GET_ENTITY_COORDS(ufo, true, 1);

	FIRE::ADD_EXPLOSION(vec.x, vec.y, vec.z, 27, 1.0f, true, false, 1.0f);

	ChaosMod::propsSet.erase(ufo);

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ufo, false, false);

	OBJECT::DELETE_OBJECT(&ufo);

	ufo = 0;

	GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectGravityField::OnActivate()
{
	Effect::OnActivate();

	entities.clear();
}

void EffectGravityField::OnTick()
{
	Effect::OnTick();

	if (TimerTick(1000))
	{
		entities.clear();

		auto peds = GetNearbyPeds(30);
		auto vehs = GetNearbyVehs(30);
		auto props = GetNearbyProps(20);


		for (auto ped : peds)
		{
			PED::SET_PED_TO_RAGDOLL(ped, 2000, 2000, 0, true, true, false);
			entities.insert(ped);
		}

		for (auto veh : vehs)
		{
			entities.insert(veh);
		}

		for (auto prop : props)
		{
			ENTITY::SET_ENTITY_DYNAMIC(prop, true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(prop, true);
			entities.insert(prop);
		}
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 vec1 = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	vec1.z += 1.0f;

	for (auto entity : entities)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(entity))
		{
			continue;
		}

		Vector3 vec2 = ENTITY::GET_ENTITY_COORDS(entity, true, 0);

		Vector3 diff = vec1;
		diff.x -= vec2.x;
		diff.y -= vec2.y;
		diff.z -= vec2.z;

		const float squareSum = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
		const float length = sqrt(squareSum);
		diff.x /= length;
		diff.y /= length;
		diff.z /= length;

		const float gravityForce = 70.0f;

		diff.x *= gravityForce;
		diff.y *= gravityForce;
		diff.z *= gravityForce;

		ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(entity, 0, diff.x, diff.y, diff.z, false, false, true, false);
	}
}

void EffectPigWeapons::OnActivate()
{
	Effect::OnActivate();

	peds.clear();
	pigs.clear();
	pigsVelocity.clear();
}

void EffectPigWeapons::OnDeactivate()
{
	Effect::OnDeactivate();

	for (auto ped : pigs)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			ChaosMod::pedsSet.erase(ped);
			PED::DELETE_PED(&ped);
		}
	}

	pigs.clear();
	peds.clear();
}

void EffectPigWeapons::OnTick()
{
	Effect::OnTick();

	if (TimerTick(2000))
	{
		peds.clear();
		auto nearbyPeds = GetNearbyPeds(20);

		Ped playerPed = PLAYER::PLAYER_PED_ID();

		nearbyPeds.push_back(playerPed);

		for (auto ped : nearbyPeds)
		{
			peds.insert(ped);
		}
	}


	static Hash unarmed = GAMEPLAY::GET_HASH_KEY((char*)"WEAPON_UNARMED");
	static Hash pigSkin = GAMEPLAY::GET_HASH_KEY((char*)"A_C_Pig_01");

	bool bModelLoaded = false;

	uint32_t timeNow = GetTickCount();

	for (uint32_t i = 0; i < pigsVelocity.size(); i++)
	{
		auto pig = pigsVelocity[i];
		if (timeNow > pig.maxTick)
		{

			pigsVelocity.erase(pigsVelocity.begin() + i);
			i--;
			continue;
		}
		PED::SET_PED_TO_RAGDOLL(pig.entity, 1000, 1000, 0, true, true, false);
		ENTITY::SET_ENTITY_VELOCITY(pig.entity, pig.velocity.x, pig.velocity.y, pig.velocity.z);
	}

	for (auto ped : peds)
	{
		Hash weaponHash = 0;

		if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponHash, 0, 0, 0) && weaponHash == unarmed)
		{
			continue;
		}

		Vector3 newVec;

		bool bFoundVec = WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(ped, &newVec);

		if (!PED::IS_PED_SHOOTING(ped) || !bFoundVec)
		{
			continue;
		}

		Vector3 pedCoord = ENTITY::GET_ENTITY_COORDS(ped, true, 0);
		pedCoord.z += 0.25f;

		Vector3 forwardVec = ENTITY::GET_ENTITY_FORWARD_VECTOR(ped);

		pedCoord.x += (forwardVec.x * 0.5f);
		pedCoord.y += (forwardVec.y * 0.5f);

		Vector3 diff = newVec;

		diff.x -= pedCoord.x;
		diff.y -= pedCoord.y;
		diff.z -= pedCoord.z;

		const float squareSum = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
		const float length = sqrt(squareSum);
		diff.x /= length;
		diff.y /= length;
		diff.z /= length;

		const float velocity = 50.0f;

		diff.x *= velocity;
		diff.y *= velocity;
		diff.z *= velocity;

		if (!bModelLoaded)
		{
			LoadModel(pigSkin);
			bModelLoaded = true;
		}

		Ped pig = PED::CREATE_PED(pigSkin, pedCoord.x, pedCoord.y, pedCoord.z, ENTITY::GET_ENTITY_HEADING(ped), true, 0, 0, 0);
		PED::SET_PED_VISIBLE(pig, true);
		ENTITY::SET_ENTITY_INVINCIBLE(pig, true);
		PED::SET_PED_CAN_RAGDOLL(pig, true);
		invoke<Void>(0x77FF8D35EEC6BBC4, pig, rand() % 4, false);

		/** _SET_PED_SCALE */
		invoke<Void>(0x25ACFC650B65C538, pig, 0.4f);

		if (ENTITY::DOES_ENTITY_EXIST(pig))
		{
			pigs.insert(pig);
			ChaosMod::pedsSet.insert(pig);
			EntityVelocity pedStruct;
			pedStruct.entity = pig;
			pedStruct.velocity = diff;
			pedStruct.maxTick = timeNow + 500;
			pigsVelocity.push_back(pedStruct);
		}

	}

	if (bModelLoaded)
	{
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pigSkin);
	}
}

void IEffectGamespeed::OnActivate()
{
	GAMEPLAY::SET_TIME_SCALE(this->timeToSet);
}

void IEffectGamespeed::OnDeactivate()
{
	GAMEPLAY::SET_TIME_SCALE(1.0f);
}

void EffectRainingPigs::OnActivate()
{
	this->pigs.clear();
}

void EffectRainingPigs::OnTick()
{
	if (!TimerTick(1000))
	{
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 vec = GetRandomCoordAroundPlayer(float(rand() % 20));

	static Hash model = GET_HASH("A_C_Pig_01");
	Ped pig = SpawnPedAroundPlayer(model, false, false);

	ENTITY::SET_ENTITY_COORDS(pig, vec.x, vec.y, vec.z + 35.0f, false, false, false, false);

	ENTITY::SET_ENTITY_INVINCIBLE(pig, true);

	/** _SET_PED_SCALE */
	invoke<Void>(0x25ACFC650B65C538, pig, float((rand() % 5) + 1));

	/** Set outfit */
	invoke<Void>(0x77FF8D35EEC6BBC4, pig, rand() % 4, false);

	PED::SET_PED_TO_RAGDOLL(pig, 10000, 10000, 0, true, true, false);

	PED::_SET_PED_RAGDOLL_BLOCKING_FLAGS(pig, 512);

	ENTITY::SET_ENTITY_VELOCITY(pig, 0.0f, 0.0f, -50.0f);

	pigs.push_back(pig);

	invoke<Void>(0x22B0D0E37CCB840D, pig, playerPed, 5000.0f, -1.0f, 0, 3.0f, 0);
}

void EffectRainingPigs::OnDeactivate()
{
	for (auto pig : pigs)
	{
		if (ENTITY::DOES_ENTITY_EXIST(pig))
		{
			ChaosMod::pedsSet.erase(pig);
			PED::DELETE_PED(&pig);
		}
	}

	this->pigs.clear();
}

void EffectRainbow::OnActivate()
{
	SetWeather(GET_HASH("FOG"));

	TIME::SET_CLOCK_TIME(8, 0, 0);

	GRAPHICS::SET_TIMECYCLE_MODIFIER((char*)"rainBowMod");
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectRainbow::OnDeactivate()
{
	GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectGiveEveryoneRifle::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(50);

	nearbyPeds.push_back(PLAYER::PLAYER_PED_ID());

	for (auto ped : nearbyPeds)
	{
		if (PED::IS_PED_HUMAN(ped))
		{
			static Hash weaponHash = GET_HASH("WEAPON_REPEATER_CARBINE");
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, weaponHash, 100, true, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(ped, weaponHash, 100);
			WEAPON::SET_CURRENT_PED_WEAPON(ped, weaponHash, true, 0, 0, 0);
		}
	}
}

void EffectTimelapse::OnActivate()
{
	SetWeather(GET_HASH("SUNNY"));

	int hours = TIME::GET_CLOCK_HOURS();
	int minutes = TIME::GET_CLOCK_MINUTES();
	int seconds = TIME::GET_CLOCK_SECONDS();

	totalSeconds = (hours * 3600) + (minutes * 60) + seconds;
}

void EffectTimelapse::OnTick()
{
	totalSeconds += ChaosMod::GetDeltaTimeSeconds() * 10000.0f;

	static float maxSeconds = float(24 * 60 * 60);

	totalSeconds = fmod(totalSeconds, maxSeconds);

	int totalSecondsInt = int(totalSeconds);

	static int secondsInMinute = 60;
	static int secondsInHour = 60 * 60;

	int hours = totalSecondsInt / secondsInHour;
	int minutes = (totalSecondsInt % secondsInHour) / secondsInMinute;
	int seconds = (totalSecondsInt % secondsInHour) % secondsInMinute;

	TIME::SET_CLOCK_TIME(hours, minutes, seconds);

	GRAPHICS::SET_TIMECYCLE_MODIFIER((char*)"SkyTimelapses01");
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectTimelapse::OnDeactivate()
{
	GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
}

void EffectNoHUD::OnTick()
{
	UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
}

void EffectInsaneGravity::OnActivate()
{
	entities.clear();
}

void EffectInsaneGravity::OnTick()
{
	if (TimerTick(1000))
	{
		entities.clear();

		auto peds = GetNearbyPeds(50);
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		peds.push_back(playerPed);
		auto vehs = GetNearbyVehs(20);
		auto props = GetNearbyProps(20);

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false))
		{
			vehs.push_back(PED::GET_VEHICLE_PED_IS_IN(playerPed, false));
		}

		for (auto ped : peds)
		{
			PED::SET_PED_GRAVITY(ped, true);
			PED::SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);
			entities.insert(ped);
		}

		for (auto veh : vehs)
		{
			entities.insert(veh);
		}

		for (auto prop : props)
		{
			entities.insert(prop);
		}
	}

	for (auto entity : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			ENTITY::SET_ENTITY_DYNAMIC(entity, true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(entity, true);
			ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(entity, 0, 0.0f, 0.0f, -200.0f, false, false, true, false);
		}
	}
}

void EffectUpsideDownCamera::OnActivate()
{
	this->cam = CAM::CREATE_CAM((char*)"DEFAULT_SCRIPTED_CAMERA", 1);
	CAM::RENDER_SCRIPT_CAMS(true, true, 500, 1, 1, 1);
}

void EffectUpsideDownCamera::OnDeactivate()
{
	CAM::SET_CAM_ACTIVE(this->cam, false);
	CAM::RENDER_SCRIPT_CAMS(false, true, 700, 1, 1, 1);
	CAM::DESTROY_CAM(this->cam, true);
	this->cam = 0;
}

void EffectUpsideDownCamera::OnTick()
{
	CAM::SET_CAM_ACTIVE(this->cam, true);

	Vector3 camCoord = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 camRotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
	camRotation.y = 180.0f;
	camRotation.x = 0.0f;
	float fov = CAM::GET_GAMEPLAY_CAM_FOV();
	CAM::SET_CAM_PARAMS(this->cam, camCoord.x, camCoord.y, camCoord.z, camRotation.x, camRotation.y, camRotation.z, fov, 0, 0, 2, 0, 0, 0);
}

void EffectOneHitKO::OnActivate()
{
	peds.clear();
}

void EffectOneHitKO::OnTick()
{
	if (TimerTick(500))
	{
		auto nearbyPeds = GetNearbyPeds(50);
		nearbyPeds.push_back(PLAYER::PLAYER_PED_ID());

		for (auto ped : nearbyPeds)
		{
			if (!peds.contains(ped))
			{
				peds.insert(ped);
			}

			if (!PED::IS_PED_DEAD_OR_DYING(ped, 1))
			{
				ENTITY::SET_ENTITY_HEALTH(ped, 1, 0);
			}
		}
	}
}

void EffectOneHitKO::OnDeactivate()
{
	for (auto ped : peds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped))
		{
			int maxHP = ENTITY::GET_ENTITY_MAX_HEALTH(ped, 1);

			ENTITY::SET_ENTITY_HEALTH(ped, maxHP, 1);
		}
	}

	peds.clear();
}