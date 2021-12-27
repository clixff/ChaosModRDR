#include "script.h"

ChaosMod* ChaosMod::Singleton = nullptr;

ChaosMod::ChaosMod()
{
}

ChaosMod::~ChaosMod()
{
}

void ChaosMod::ScriptMain()
{
	srand(GetTickCount());

	/** Destroy old mod instance */
	if (ChaosMod::Singleton)
	{
		delete ChaosMod::Singleton;
	}

	/** Create mod instance and activate it */
	ChaosMod::Singleton = new ChaosMod();

	ChaosMod::Get()->Main();
}

ChaosMod* ChaosMod::Get()
{
	return ChaosMod::Singleton;
}

bool ChaosMod::IsModEnabled()
{
	return bEnabled;
}

void ChaosMod::DrawText(char* text, Vector2 position, Vector2 scale, LinearColor color, bool bCenter, int shadowSize, LinearColor shadowColor)
{
	UI::SET_TEXT_SCALE(scale.X, scale.Y);
	UI::SET_TEXT_COLOR_RGBA(color.R, color.G, color.B, color.A);
	UI::SET_TEXT_CENTRE(bCenter);
	UI::SET_TEXT_DROPSHADOW(shadowSize, shadowColor.R, shadowColor.G, shadowColor.B, shadowColor.A);;

	char* varString = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", text);

	UI::DRAW_TEXT(varString, 0.5f, 0.5f);
}

void ChaosMod::OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key < 0xFF)
	{
		auto* modInstance = ChaosMod::Get();

		if (!modInstance)
		{
			return;
		}

		modInstance->keyStates[key].time = GetTickCount();
		modInstance->keyStates[key].bPressed = isUpNow;
	}
}

void ChaosMod::ToggleModStatus()
{
	bEnabled = !bEnabled;

	if (bEnabled)
	{
		ResetEffectsTimeout();

		ModUITextString = "Chaos Mod enabled";

		Ped playerPed = PLAYER::PLAYER_PED_ID();

		Vector3 playerVelocity = ENTITY::GET_ENTITY_VELOCITY(playerPed, 0);

		PED::SET_PED_TO_RAGDOLL(playerPed, 5000, 5000, 0, true, true, false);

		ENTITY::SET_ENTITY_VELOCITY(playerPed, playerVelocity.x, playerVelocity.y, 20.0f);
	}
	else
	{
		ModUITextString = "Chaos Mod disabled";
	}

	ModUITextEndTime = GetTickCount() + 3000;
}

void ChaosMod::Main()
{
	while (true)
	{
		Update();
		InputTick();
		WAIT(0);
	}
}

void ChaosMod::Update()
{
	/** Draw mod UI text */
	if (GetTickCount() < ModUITextEndTime)
	{
		ChaosMod::DrawText((char*)ModUITextString.c_str(), Vector2(0.5f, 0.5f), Vector2(0.55f, 0.55f), LinearColor(255, 255, 255, 255), false, 5, LinearColor(0, 0, 0, 255));
	}

	if (!bEnabled)
	{
		return;
	}

	if (timeoutVotingStartTime && !bVotingEnabled && GetTickCount() >= timeoutVotingStartTime)
	{
		/** TODO: Activate voting */

		bVotingEnabled = true;
	}

	if (timeoutEndTime && GetTickCount() >= timeoutEndTime)
	{
		ResetEffectsTimeout();
		/** TODO: Activate selected effect */
	}

	DrawUI();
}

void ChaosMod::InputTick()
{
	if (isKeyPressed(VK_F7))
	{
		ToggleModStatus();
	}
}

void ChaosMod::DrawUI()
{
	/** 16px on 1080p */
	static const float ProgressBarHeight = 16.0f / 1080.f;

	/** Draw progress bar background */
	GRAPHICS::DRAW_RECT(0.5f, ProgressBarHeight / 2.0f, 1.0f, ProgressBarHeight, 0, 0, 0, 168, 0, 0);

	float effectTimeoutValue = float(float(GetTickCount() - timeoutStartTime) / float(timeoutEndTime - timeoutStartTime));

	if (effectTimeoutValue < 0.0f) effectTimeoutValue = 0.0f;
	else if (effectTimeoutValue > 1.0f) effectTimeoutValue = 1.0f;

	/** Draw progress bar foreground */
	GRAPHICS::DRAW_RECT(effectTimeoutValue / 2.0f, ProgressBarHeight / 2.0f, effectTimeoutValue, ProgressBarHeight, 143, 6, 6, 255, 0, 0);
}

bool ChaosMod::isKeyPressed(uint8_t key)
{
	bool bPressed = keyStates[key].bPressed && GetTickCount() < keyStates[key].time + 100;

	if (bPressed)
	{
		keyStates[key].bPressed = false;
		keyStates[key].time = 0;
	}
	
	return bPressed;
}

void ChaosMod::ResetEffectsTimeout()
{
	bVotingEnabled = false;

	timeoutStartTime = GetTickCount();
	timeoutEndTime = timeoutStartTime + (effectsInterval * 1000);
	timeoutVotingStartTime = timeoutEndTime - (effectsVoteTime * 1000);
}