#include "misc.h"

void EffectSpawnHotchkissCannon::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerLocation = ENTITY::GET_ENTITY_COORDS(playerPed, true, 0);

	static Hash model = GAMEPLAY::GET_HASH_KEY((char*)"hotchkiss_cannon");

	STREAMING::REQUEST_MODEL(model, true);

	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	VEHICLE::CREATE_VEHICLE(model, playerLocation.x, playerLocation.y, playerLocation.y, rand() % 360, false, false, false, false);
}
