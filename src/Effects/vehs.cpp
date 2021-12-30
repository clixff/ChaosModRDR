#include "vehs.h"
#include "peds.h"

std::vector<Vehicle> GetNearbyVehs(int32_t Max)
{
	std::vector<Vehicle> vehsOut;
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	NearbyEntities vehs;
	vehs.size = 100;

	if (Max > 100)
	{
		Max = 100;
	}

	int pedsFound = PED::GET_PED_NEARBY_VEHICLES(playerPed, (int*)&vehs);

	if (Max > pedsFound)
	{
		Max = pedsFound;
	}

	bool bPlayerInVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, true);

	Vehicle playerVeh = 0;
	if (bPlayerInVehicle)
	{
		playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	}

	for (int32_t i = 0; i < Max; i++)
	{
		Entity veh = vehs.entities[i];
		if (ENTITY::DOES_ENTITY_EXIST(veh) && (!bPlayerInVehicle || veh != playerVeh))
		{
			vehsOut.push_back(veh);
		}
	}

	return vehsOut;
}

void EffectSpawnWagon::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"COACH3");

	LoadModel(model);

	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, rand() % 360, false, false, false, false);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
}

void EffectFlipAllVehs::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(45);
	auto nearbyVehs = GetNearbyVehs(45);

	std::vector<Entity> entities = nearbyVehs;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		entities.push_back(veh);
	}

	for (auto ped : nearbyPeds)
	{
		Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

		/** IS_MODEL_A_HORSE */
		bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);

		if (bModelIsHorse)
		{
			PED::SET_PED_TO_RAGDOLL(ped, 5000, 5000, 0, true, true, false);
			entities.push_back(ped);
		}
	}

	for (auto entity : entities)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			Vector3 rotation = ENTITY::GET_ENTITY_ROTATION(entity, 2);

			Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(entity, 0);

			ENTITY::SET_ENTITY_ROTATION(entity, rotation.x + 180.0f, rotation.y, rotation.z, 2, true);

			ENTITY::SET_ENTITY_VELOCITY(entity, velocity.x, velocity.y, velocity.z + 5.0f);
		}
	}
}

void EffectMinecartRain::OnActivate()
{
	vehs.clear();
}

void EffectMinecartRain::OnDeactivate()
{
	for (auto veh : vehs)
	{
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			VEHICLE::DELETE_VEHICLE(&veh);
		}
	}

	vehs.clear();
}

void EffectMinecartRain::OnTick()
{
	Effect::OnTick();

	if (GetTickCount() % 1000)
	{
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash cartHash = GAMEPLAY::GET_HASH_KEY((char*)"mineCart01x");

	LoadModel(cartHash);

	float radius = float(rand() % 20);

	/** In radians */
	float angle = float(rand() % 360) * (M_PI / 180.0f);

	playerLocation.x += radius * sin(angle);
	playerLocation.y += radius * cos(angle);

	Vehicle veh = VEHICLE::CREATE_VEHICLE(cartHash, playerLocation.x, playerLocation.y, playerLocation.z + 35.0f, rand() % 360, false, false, false, false);

	ENTITY::SET_ENTITY_VELOCITY(veh, 0.0f, 0.0f, -150.0f);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(cartHash);

	vehs.push_back(veh);
}

void EffectFullAcceleration::OnActivate()
{
	vehs.clear();
	horses.clear();
}

void EffectFullAcceleration::OnDeactivate()
{
	vehs.clear();
	
	for (auto horse : horses)
	{
		if (ENTITY::DOES_ENTITY_EXIST(horse))
		{
			ENTITY::SET_ENTITY_INVINCIBLE(horse, false);
		}
	}

	horses.clear();
}

void EffectFullAcceleration::OnTick()
{
	if (GetTickCount() % 1000 == 0)
	{
		vehs.clear();

		vehs = GetNearbyVehs(45);

		Ped playerPed = PLAYER::PLAYER_PED_ID();

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
		{
			vehs.push_back(PED::GET_VEHICLE_PED_IS_IN(playerPed, false));
		}

		auto peds = GetNearbyPeds(45);

		for (auto ped : peds)
		{
			Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);
			/** IS_MODEL_A_HORSE */
			bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);

			if (bModelIsHorse)
			{
				Vector3 horseForwardVec = ENTITY::GET_ENTITY_FORWARD_VECTOR(ped);

				float acceleration = 1000.0f;

				horseForwardVec.x *= acceleration;
				horseForwardVec.y *= acceleration;
				horseForwardVec.z *= acceleration;

				ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
				horses.insert(ped);

				PED::SET_PED_TO_RAGDOLL(ped, 1000, 1000, 0, true, true, false);

				ENTITY::SET_ENTITY_VELOCITY(ped, horseForwardVec.x, horseForwardVec.y, horseForwardVec.z);
			}
		}
	}

	for (auto veh : vehs)
	{
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 100.0f);
		}
	}
}

void EffectEveryoneExitsVehs::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	auto peds = GetNearbyPeds(45);

	peds.push_back(playerPed);

	for (auto ped : peds)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, true))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
			AI::TASK_LEAVE_VEHICLE(ped, veh, 4160, 0);
		}
		else if (PED::IS_PED_ON_MOUNT(ped))
		{
			/** _REMOVE_PED_FROM_MOUNT */
			invoke<Void>(0x5337B721C51883A9, ped, 0, 0);
		}
	}
}

void EffectSetPlayerIntroRandomVeh::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<Entity> entities;

	auto peds = GetNearbyPeds(45);
	auto vehs = GetNearbyVehs(45);

	entities = vehs;

	for (auto ped : peds)
	{
		Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);
		/** IS_MODEL_A_HORSE */
		bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, pedModel);

		if (bModelIsHorse)
		{
			entities.push_back(ped);
		}
	}

	/** No vehicles nearby */
	if (entities.size() == 0)
	{
		return;
	}

	Entity entityToUse = entities[rand() % entities.size()];
	int32_t seatIndex = rand() % 2 ? -1 : -2;

	if (ENTITY::IS_ENTITY_A_PED(entityToUse))
	{
		/** __SET_PED_ON_MOUNT */
		invoke<Void>(0x028F76B6E78246EB, playerPed, entityToUse, seatIndex, true);
	}
	else
	{
		PED::SET_PED_INTO_VEHICLE(playerPed, entityToUse, seatIndex);
	}
}

void EffectSpawnHotAirBalloon::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"hotAirBalloon01");

	LoadModel(model);

	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, rand() % 360, false, false, false, false);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);

	Vector3 windDirection = GAMEPLAY::GET_WIND_DIRECTION();

	windDirection.x *= 25.0f;
	windDirection.y *= 25.0f;
	windDirection.z = 25.0f;

	ENTITY::SET_ENTITY_VELOCITY(veh, windDirection.x, windDirection.y, 25.0f);
}

void EffectIgnitePlayersWagon::OnActivate()
{
	Effect::OnActivate();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		VEHICLE::EXPLODE_VEHICLE(veh, true, false, 0);
	}
}

void EffectIgniteNearbyWagons::OnActivate()
{
	Effect::OnActivate();

	auto vehs = GetNearbyVehs(45);

	for (auto veh : vehs)
	{
		VEHICLE::EXPLODE_VEHICLE(veh, true, false, 0);
	}
}

void EffectSpawnCanoe::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"CANOE");

	LoadModel(model);

	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, rand() % 360, false, false, false, false);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
}

void EffectHorsesRain::OnActivate()
{
	Effect::OnActivate();

	horses.clear();
}

void EffectHorsesRain::OnDeactivate()
{
	Effect::OnDeactivate();

	for (auto horse : horses)
	{
		if (ENTITY::DOES_ENTITY_EXIST(horse))
		{
			PED::DELETE_PED(&horse);
		}
	}

	horses.clear();
}

void EffectHorsesRain::OnTick()
{
	Effect::OnTick();

	if (GetTickCount() % 1000)
	{
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	std::vector<const char*> models = {
		"A_C_Horse_Morgan_Bay",
		"A_C_Horse_Arabian_Black",
		"A_C_Horse_Arabian_Grey",
		"A_C_Horse_Arabian_White",
		"A_C_Horse_Shire_DarkBay",
		"A_C_Horse_TennesseeWalker_DappleBay"
	};
	
	Hash skinHash = GAMEPLAY::GET_HASH_KEY((char*)models[rand() % models.size()]);

	Ped horse = SpawnPedAroundPlayer(skinHash, false);
	ENTITY::SET_ENTITY_INVINCIBLE(horse, true);
	PED::SET_PED_CAN_RAGDOLL(horse, false);

	float radius = float(rand() % 20);

	/** In radians */
	float angle = float(rand() % 360) * (M_PI / 180.0f);

	playerLocation.x += radius * sin(angle);
	playerLocation.y += radius * cos(angle);

	ENTITY::SET_ENTITY_COORDS(horse, playerLocation.x, playerLocation.y, playerLocation.z + 35.0f, false, false, false, false);
	ENTITY::SET_ENTITY_VELOCITY(horse, 0.0f, 0.0f, -60.0f);

	horses.push_back(horse);
}
