#include "vehs.h"
#include "peds.h"
#include "../script.h"
#include <algorithm>
#include <random>
#include "misc.h"

std::vector <Vehicle> GetNearbyVehs(int32_t Max)
{
	std::vector <Vehicle> vehsOut;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	NearbyEntities vehs;
	vehs.size = 100;
	
	if (Max > 100)
	{
		Max = 100;
	}
	
	int pedsFound = PED::GET_PED_NEARBY_VEHICLES(playerPed, (int*) &vehs);
	
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
	
	static Hash model = GAMEPLAY::GET_HASH_KEY((char*) "COACH3");
	
	LoadModel(model);
	
	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, playerHeading,
										  false, false, false, false);
	DECORATOR::DECOR_SET_BOOL(veh, (char*) "wagon_block_honor", true);
	
	Vehicle vehCopy = veh;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	
	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
	
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}
}

void EffectFlipAllVehs::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(45);
	auto nearbyVehs = GetNearbyVehs(45);
	
	std::vector <Entity> entities = nearbyVehs;
	
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

void EffectTrainsawLaser::OnActivate()
{
	vehs.clear();
}

void EffectTrainsawLaser::OnDeactivate()
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

void EffectTrainsawLaser::OnTick()
{
	Effect::OnTick();
	
	if (!TimerTick(500))
	{
		return;
	}
	
	static Hash trainHash = GAMEPLAY::GET_HASH_KEY((char*) "CABOOSE01X");
	
	LoadModel(trainHash);
	
	Vehicle veh = VEHICLE::CREATE_VEHICLE(trainHash, 0, 0, 35.0f, rand() % 360, false, false, false, false);
	
	ENTITY::SET_ENTITY_VELOCITY(veh, 0.0f, 0.0f, -100.0f);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(trainHash);
	
	vehs.push_back(veh);
	
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
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
	
	if (!TimerTick(500))
	{
		return;
	}
	
	static Hash cartHash = GAMEPLAY::GET_HASH_KEY((char*) "mineCart01x");
	
	LoadModel(cartHash);
	
	Vector3 vec = GetRandomCoordAroundPlayer(float(rand() % 20));
	
	Vehicle veh = VEHICLE::CREATE_VEHICLE(cartHash, vec.x, vec.y, vec.z + 35.0f, rand() % 360, false, false, false,
										  false);
	
	ENTITY::SET_ENTITY_VELOCITY(veh, 0.0f, 0.0f, -150.0f);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(cartHash);
	
	vehs.push_back(veh);
	
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}
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
	if (TimerTick(1000))
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
			
			Hash vehModel = ENTITY::GET_ENTITY_MODEL(veh);
			
			bool bModelIsTrain = invoke<bool>(0xFC08C8F8C1EDF174, vehModel);
			
			if (bModelIsTrain)
			{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 1000.0f);
				VEHICLE::SET_TRAIN_SPEED(veh, 1000.0f);
				VEHICLE::SET_TRAIN_CRUISE_SPEED(veh, 1000.0f);
			}
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

void EffectSetPlayerIntoRandomVeh::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	std::vector <Entity> entities;
	
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
	
	auto randomEngine = std::default_random_engine{};
	std::shuffle(std::begin(entities), std::end(entities), randomEngine);
	
	for (auto entity : entities)
	{
		if (ENTITY::IS_ENTITY_A_PED(entity))
		{
			std::vector <int32_t> seatIndices = {-1, 0};
			
			for (auto index : seatIndices)
			{
				bool bIsMountSeatFree = invoke<bool>(0xAAB0FE202E9FC9F0, entity, index);
				
				if (bIsMountSeatFree)
				{
					/** __SET_PED_ON_MOUNT */
					invoke<Void>(0x028F76B6E78246EB, playerPed, entity, index, true);
					return;
				}
			}
			
		}
		else
		{
			bool bSeatFree = VEHICLE::IS_VEHICLE_SEAT_FREE(entity, -1);
			
			if (bSeatFree)
			{
				PED::SET_PED_INTO_VEHICLE(playerPed, entity, -1);
				return;
			}
			
			auto hash = ENTITY::GET_ENTITY_MODEL(entity);
			
			int32_t seatsNum = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(hash) - 1;
			
			for (int32_t i = 0; i < seatsNum; i++)
			{
				bSeatFree = VEHICLE::IS_VEHICLE_SEAT_FREE(entity, i);
				
				if (bSeatFree)
				{
					PED::SET_PED_INTO_VEHICLE(playerPed, entity, i);
					return;
				}
			}
		}
	}
}

void EffectSpawnHotAirBalloon::OnActivate()
{
	Effect::OnActivate();
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	
	playerLocation.z += 2.0f;
	
	static Hash model = GAMEPLAY::GET_HASH_KEY((char*) "hotAirBalloon01");
	
	LoadModel(model);
	
	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, rand() % 360,
										  false, false, false, false);
	
	Vehicle vehCopy = veh;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	
	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
	
	Vector3 windDirection = GAMEPLAY::GET_WIND_DIRECTION();
	
	windDirection.x *= 25.0f;
	windDirection.y *= 25.0f;
	windDirection.z = 25.0f;
	
	ENTITY::SET_ENTITY_VELOCITY(veh, windDirection.x, windDirection.y, 25.0f);
	
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}
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
	
	static Hash model = GAMEPLAY::GET_HASH_KEY((char*) "CANOE");
	
	LoadModel(model);
	
	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, rand() % 360,
										  false, false, false, false);
	
	Vehicle vehCopy = veh;
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&vehCopy);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	
	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
	
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}
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
			ChaosMod::pedsSet.erase(horse);
			PED::DELETE_PED(&horse);
		}
	}
	
	horses.clear();
}

void EffectHorsesRain::OnTick()
{
	Effect::OnTick();
	
	if (!TimerTick(1000))
	{
		return;
	}
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	std::vector<const char*> models = {"A_C_Horse_Morgan_Bay", "A_C_Horse_Arabian_Black", "A_C_Horse_Arabian_Grey",
									   "A_C_Horse_Arabian_White", "A_C_Horse_Shire_DarkBay",
									   "A_C_Horse_TennesseeWalker_DappleBay"};
	
	Hash skinHash = GAMEPLAY::GET_HASH_KEY((char*) models[rand() % models.size()]);
	
	Ped horse = SpawnPedAroundPlayer(skinHash, false);
	ENTITY::SET_ENTITY_INVINCIBLE(horse, true);
	PED::SET_PED_CAN_RAGDOLL(horse, false);
	
	Vector3 vec = GetRandomCoordAroundPlayer(float(rand() % 20));
	
	ENTITY::SET_ENTITY_COORDS(horse, vec.x, vec.y, vec.z + 35.0f, false, false, false, false);
	ENTITY::SET_ENTITY_VELOCITY(horse, 0.0f, 0.0f, -40.0f);
	
	horses.push_back(horse);
}

void EffectDetachWheels::OnActivate()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		
		for (int32_t i = 0; i < 4; i++)
		{
			invoke<Void>(0xd4f5efb55769d272, veh, i);
		}
	}
}

void EffectSetPedsIntoPlayerVehicle::OnActivate()
{
	Effect::OnActivate();
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	auto nearbyPeds = GetNearbyPeds(30);
	
	bool bIsMount = false;
	
	Entity veh = 0;
	int32_t seatsNum = 0;
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		
		auto hash = ENTITY::GET_ENTITY_MODEL(veh);
		
		seatsNum = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(hash) - 1;
	}
	else if (PED::IS_PED_ON_MOUNT(playerPed))
	{
		bIsMount = true;
		veh = PED::GET_MOUNT(playerPed);
	}
	else
	{
		return;
	}
	
	for (auto ped : nearbyPeds)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			auto hash = ENTITY::GET_ENTITY_MODEL(ped);
			
			/** IS_MODEL_A_HORSE */
			bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, hash);
			
			if (bModelIsHorse)
			{
				continue;
			}
			
			if (bIsMount)
			{
				bool bIsMountSeatFree = invoke<bool>(0xAAB0FE202E9FC9F0, veh, 0);
				if (bIsMountSeatFree)
				{
					/** __SET_PED_ON_MOUNT */
					invoke<Void>(0x028F76B6E78246EB, ped, veh, 0, true);
				}
				
				return;
			}
			else
			{
				for (int32_t i = 0; i < seatsNum; i++)
				{
					bool bIsSeatFree = VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i);
					
					if (bIsSeatFree)
					{
						PED::SET_PED_INTO_VEHICLE(ped, veh, i);
						break;
					}
					
					if (i == seatsNum - 1)
					{
						return;
					}
				}
			}
		}
	}
}

void EffectFastPlayersWagon::OnTick()
{
	float maxSpeed = 25.0f;
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		
		float speed = ENTITY::GET_ENTITY_SPEED(veh);
		
		float newSpeed = speed * 2.0f;
		
		
		Hash vehModel = ENTITY::GET_ENTITY_MODEL(veh);
		
		bool bModelIsTrain = invoke<bool>(0xFC08C8F8C1EDF174, vehModel);
		
		if (bModelIsTrain)
		{
			maxSpeed = 1000.0f;
		}
		
		if (newSpeed > maxSpeed)
		{
			newSpeed = maxSpeed;
		}
		
		VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, newSpeed);
		
		
		if (bModelIsTrain)
		{
			VEHICLE::SET_TRAIN_SPEED(veh, newSpeed);
			VEHICLE::SET_TRAIN_CRUISE_SPEED(veh, newSpeed);
		}
	}
}

void EffectRandomWheelsDetaching::OnActivate()
{
	auto nearbyVehs = GetNearbyVehs(50);
	
	auto playerPed = PLAYER::PLAYER_PED_ID();
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		nearbyVehs.push_back(PED::GET_VEHICLE_PED_IS_IN(playerPed, false));
	}
	
	for (auto veh : nearbyVehs)
	{
		for (int32_t i = 0; i < 4; i++)
		{
			if (rand() % 2)
			{
				invoke<Void>(0xd4f5efb55769d272, veh, i);
			}
		}
	}
}

void EffectSpawnRandomVeh::OnActivate()
{
	static std::vector<const char*> vehs = {"CART01", "CART02", "CART03", "CART04", "CART05", "CART06", "CART07",
											"CART08", "ARMYSUPPLYWAGON", "BUGGY01", "BUGGY02", "BUGGY03",
											"CHUCKWAGON000X", "CHUCKWAGON002X", "COACH2", "COACH3", "COACH4", "COACH5",
											"COACH6", "coal_wagon", "OILWAGON01X", "POLICEWAGON01X", "WAGON02X",
											"WAGON04X", "LOGWAGON", "WAGON03X", "WAGON05X", "WAGON06X",
											"WAGONPRISON01X", "STAGECOACH001X", "STAGECOACH002X", "UTILLIWAG",
											"GATCHUCK", "GATCHUCK_2", "wagonCircus01x", "wagonDairy01x", "wagonWork01x",
											"wagonTraveller01x", "KEELBOAT", "CANOE", "CANOETREETRUNK", "SKIFF",
											"BREACH_CANNON", "trolley01x"};
	
	Hash model = GET_HASH(vehs[rand() % vehs.size()]);
	
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);
	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	
	LoadModel(model);
	
	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.z, playerHeading,
										  false, false, false, false);
	
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		ChaosMod::vehsSet.insert(veh);
	}
	
	PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
}

void EffectHorsesAreDonkeys::OnActivate()
{
	auto nearbyPeds = GetNearbyPeds(140);
	
	static Hash donkeyModel = GET_HASH("A_C_Donkey_01");
	
	for (auto ped : nearbyPeds)
	{
		Hash model = ENTITY::GET_ENTITY_MODEL(ped);
		
		/** IS_MODEL_A_HORSE */
		bool bModelIsHorse = invoke<bool>(0x772A1969F649E902, model);
		if (bModelIsHorse)
		{
			Vector3 vec = ENTITY::GET_ENTITY_COORDS(ped, true, 0);
			Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(ped, 1);
			float heading = ENTITY::GET_ENTITY_HEADING(ped);
			
			Ped rider = 0;
			
			/** _IS_MOUNT_SEAT_FREE */
			if (!PED::_0xAAB0FE202E9FC9F0(ped, -1))
			{
				/** _GET_RIDER_OF_MOUNT */
				rider = PED::_0xB676EFDA03DADA52(ped, 0);
				/** _REMOVE_PED_FROM_MOUNT */
				invoke<Void>(0x5337B721C51883A9, rider, 0, 0);
			}
			
			if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, false, false);
				
				PED::DELETE_PED(&ped);
			}
			
			Ped donkey = SpawnPedAroundPlayer(donkeyModel, false, false);
			ENTITY::SET_ENTITY_COORDS(donkey, vec.x, vec.y, vec.z, false, false, false, false);
			ENTITY::SET_ENTITY_VELOCITY(donkey, vel.x, vel.y, vel.z);
			ENTITY::SET_ENTITY_HEADING(donkey, heading);
			
			if (rider)
			{
				/** __SET_PED_ON_MOUNT */
				invoke<Void>(0x028F76B6E78246EB, rider, donkey, -1, true);
			}
		}
	}
}

void EffectOilWagonsRain::OnActivate()
{
	vehs.clear();
	notExplodedVehs.clear();
}

void EffectOilWagonsRain::OnDeactivate()
{
	for (auto veh : vehs)
	{
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			VEHICLE::DELETE_VEHICLE(&veh);
		}
	}
	
	vehs.clear();
	notExplodedVehs.clear();
}

void EffectOilWagonsRain::OnTick()
{
	if (TimerTick(2500))
	{
		static Hash model = GET_HASH("OilWagon01X");
		LoadModel(model);
		
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vector3 vec = GetRandomCoordAroundPlayer(float(rand() % 50));
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, vec.x, vec.y, vec.z + 175.0f, 0.0f, 0, 0, true, 0);
		
		ENTITY::SET_ENTITY_ROTATION(veh, 0.0f, 180.0f, 0.0f, 2, 1);
		ENTITY::SET_ENTITY_VELOCITY(veh, 0.0f, 0.0f, -150.0f);
		
		vehs.push_back(veh);
		notExplodedVehs.insert(veh);
		
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			ChaosMod::vehsSet.insert(veh);
		}
		
		
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}
	
	for (auto veh : notExplodedVehs)
	{
		Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(veh, 0);
		if (vel.z >= -0.5f)
		{
			VEHICLE::EXPLODE_VEHICLE(veh, 1, 0, 0);
			notExplodedVehs.erase(veh);
		}
	}
}
