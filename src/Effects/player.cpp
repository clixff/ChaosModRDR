#include "player.h"

void EffectLaunchPlayerUp::OnActivate()
{
	Effect::OnActivate();

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerVelocity = ENTITY::GET_ENTITY_VELOCITY(playerPed, 0);

	PED::SET_PED_TO_RAGDOLL(playerPed, 5000, 5000, 0, true, true, false);
	ENTITY::SET_ENTITY_VELOCITY(playerPed, playerVelocity.x, playerVelocity.y, 35.0f);
}
