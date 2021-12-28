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
