#include "script.h"
#include "Effects/peds.h"
#include "Effects/player.h"
#include "Effects/misc.h"

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

		for (auto* effect : activeEffects)
		{
			effect->OnActivate();
		}
	}
	else
	{
		ModUITextString = "Chaos Mod disabled";

		activeEffects = {};
	}

	ModUITextEndTime = GetTickCount() + 3000;
}

void ChaosMod::ActivateEffect(Effect* effect)
{
	if (!effect)
	{
		return;
	}

	effect->OnActivate();

	this->activeEffects.push_back(effect);
}

void ChaosMod::ActivateRandomEffect()
{
	if (AllEffects.size() == 0)
	{
		return;
	}

	int effectID = rand() % AllEffects.size();

	ActivateEffect(AllEffects[effectID]);
}

void ChaosMod::Main()
{
	InitEffects();

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
		ChaosMod::DrawText((char*)ModUITextString.c_str(), Vector2(0.5f, 0.5f), Vector2(0.55f, 0.55f), LinearColor(255, 255, 255, 255), true, 5, LinearColor(0, 0, 0, 255));
	}

	if (!bEnabled)
	{
		return;
	}

	if (timeoutVotingStartTime && !bVotingEnabled && GetTickCount() >= timeoutVotingStartTime)
	{
		/** TODO: Activate voting */

		bVotingEnabled = true;

		for (int32_t i = 0; i < activeEffects.size(); i++)
		{
			auto* effect = activeEffects[i];

			if (effect && !effect->bTimed)
			{
				effect->OnDeactivate();
				activeEffects.erase(activeEffects.begin() + i);
				i--;
			}
		}
	}

	if (timeoutEndTime && GetTickCount() >= timeoutEndTime)
	{
		ResetEffectsTimeout();
		/** TODO: Activate selected effect */
		ActivateRandomEffect();
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

	for (int32_t i = 0; i < activeEffects.size(); i++)
	{
		DrawEffectInUI(activeEffects[i], i);
	}
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

void ChaosMod::DrawEffectInUI(Effect* effect, int32_t index)
{
	if (!effect)
	{
		return;
	}

	float X = 1690.0f / 1920.0f;
	float minY = 378.0f;
	float margin = 52.0f;
	float Y = (minY + (index * margin)) / 1080.0f;

	if (effect->bTimed)
	{
		GRAPHICS::DRAW_RECT(X, Y, 400.0f / 1920.0f, 40.0f / 1080.0f, 0, 0, 0, 120, 0, 0);
	}

	UI::SET_TEXT_SCALE(0.0f, 0.35f);
	UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 240);
	UI::SET_TEXT_CENTRE(1);
	UI::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 255);

	char* varString = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)effect->name.c_str());

	UI::DRAW_TEXT(varString, X, Y - 0.0125f);
}

void ChaosMod::InitEffects()
{
	AllEffects = {
		new EffectSpawnSoldier(),
		new EffectSpawnLenny(),
		new EffectSpawnDrunkardJon(),
		new EffectSpawnChicken(),
		new EffectLaunchPlayerUp(),
		new EffectSpawnHotchkissCannon()
	};

	EffectsMap.clear();

	for (auto* effect : AllEffects)
	{
		EffectsMap.emplace(effect->ID, effect);
	}
}