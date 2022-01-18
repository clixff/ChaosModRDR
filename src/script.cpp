#include "script.h"
#include "Effects/peds.h"
#include "Effects/player.h"
#include "Effects/misc.h"
#include "Effects/vehs.h"
#include <tlhelp32.h>
#include <filesystem>
#include <fstream>

ChaosMod* ChaosMod::Singleton = nullptr;
std::mutex ChaosMod::globalMutex = std::mutex();
uint32_t ChaosMod::LastTick = 0;
uint64_t ChaosMod::ThreadID = 0;
Ped ChaosMod::PLAYER_PED = 0;
HMODULE ChaosMod::hInstance = 0;

std::set<Ped> ChaosMod::pedsSet = std::set<Ped>();
std::set<Vehicle> ChaosMod::vehsSet = std::set<Vehicle>();
std::set<Entity> ChaosMod::propsSet = std::set<Entity>();

Hash ChaosMod::PlayerSkin1 = 0;
Hash ChaosMod::PlayerSkin2 = 0;

uint32_t ChaosMod::_DeltaTime = 0;
float ChaosMod::_DeltaTimeSeconds = 0.0f;
std::string ChaosMod::logString = "";

ChaosMod::ChaosMod()
{
}

ChaosMod::~ChaosMod()
{
	if (notificationData)
	{
		delete notificationData;
	}

	if (notificationDuration)
	{
		delete notificationDuration;
	}
}

void ChaosMod::ScriptMain()
{
	srand(GetTickCount());

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
	ChaosMod::globalMutex.lock();

	bEnabled = !bEnabled;

	if (bEnabled)
	{
		bool bPrevTwitchStatus = config.bTwitch;

		config.Read();

		ResetEffectsTimeout();

		ResetMetaTimer();

		if (bPrevTwitchStatus != config.bTwitch)
		{
			if (config.bTwitch)
			{
				StartWSServer();
			}
			else
			{
				ChaosMod::StopServer();
			}
		}
		else
		{
			if (config.bTwitch && wsServer)
			{
				wsServer->SendMessageToClient("mod_enabled");
			}
		}

		std::string effectsNumStr = "~q~Loaded ~COLOR_GOLD~" + std::to_string(AllEffects.size()) + "~q~ effects";

		ShowNotification2("~COLOR_GREEN~Chaos Mod Enabled", effectsNumStr.c_str(), 3500, "scoretimer_textures", "scoretimer_generic_tick", LinearColor(77, 170, 104, 255));
	}
	else
	{
		for (auto* effect : activeEffects)
		{
			effect->OnDeactivate();
		}

		activeEffects = {};

		if (config.bTwitch && wsServer)
		{
			wsServer->SendMessageToClient("mod_disabled");
		}

		ShowNotification2("~COLOR_PLAYER_STATUS_NEGATIVE~Chaos Mod Disabled", "", 3500, "scoretimer_textures", "scoretimer_generic_cross", LinearColor(204, 0, 0, 255));

		for (auto ped : ChaosMod::pedsSet)
		{
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, false, false);
				PED::DELETE_PED(&ped);
			}
		}

		for (auto veh : ChaosMod::vehsSet)
		{
			if (ENTITY::DOES_ENTITY_EXIST(veh))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, false);
				VEHICLE::DELETE_VEHICLE(&veh);
			}
		}

		for (auto prop : ChaosMod::propsSet)
		{
			if (ENTITY::DOES_ENTITY_EXIST(prop))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(prop, false, false);
				OBJECT::DELETE_OBJECT(&prop);
			}
		}

		ChaosMod::ResetPlayerSkin();
	}

	AUDIO::PLAY_SOUND_FRONTEND((char*)"SELECT", (char*)"HUD_SHOP_SOUNDSET", true, 0);
	
	ChaosMod::pedsSet.clear();
	ChaosMod::vehsSet.clear();
	ChaosMod::propsSet.clear();

	oldEffects.clear();

	activeMeta = nullptr;

	ChaosMod::globalMutex.unlock();
}

void ChaosMod::ActivateEffect(Effect* effect)
{
	if (!effect)
	{
		return;
	}

	if (effect->bTimed)
	{
		effect->ActivationTime = GetTickCount();
		effect->DeactivationTime = effect->ActivationTime + (effect->EffectDuration * 1000);
	}

	prevActivatedEffect = effect;

	effect->OnActivate();

	if (effect->bIsMeta)
	{
		this->activeEffects.insert(activeEffects.begin(), effect);
	}
	else
	{
		this->activeEffects.push_back(effect);
	}
}

void ChaosMod::StartNodeProcess()
{
	LogToFile("Starting node process..");

	ZeroMemory(&this->NodeStartupInfo, sizeof(this->NodeStartupInfo));
	NodeStartupInfo.cb = sizeof(this->NodeStartupInfo);
	ZeroMemory(&this->NodeProcessInformation, sizeof(this->NodeProcessInformation));

	WCHAR gameDir[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, gameDir);

	std::wstring chaosDir = gameDir;

	chaosDir += L"\\ChaosMod";

	std::wstring exePath = chaosDir;

	exePath += L"\\ChaosModRDRTwitch.exe";

	LPWSTR exePath_c = const_cast<wchar_t*>(exePath.c_str());
	LPWSTR chaosDir_c = const_cast<wchar_t*>(chaosDir.c_str());

	auto processCreationResult = CreateProcessW(NULL, exePath_c, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, chaosDir_c, &NodeStartupInfo, &NodeProcessInformation);

	std::string logStr = "Node process started: " + std::to_string(processCreationResult);

	LogToFile(logStr.c_str());
}

void ChaosMod::TerminateNodeProcess()
{
	PROCESSENTRY32W processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32W);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32FirstW(snapshot, &processEntry) == TRUE)
	{
		while (Process32NextW(snapshot, &processEntry) == TRUE)
		{
			if (_wcsicmp(processEntry.szExeFile, L"ChaosModRDRTwitch.exe") == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);

				TerminateProcess(hProcess, 0);

				CloseHandle(hProcess);
			}
		}
	}

	CloseHandle(snapshot);
}

void ChaosMod::StopServer()
{
	auto* mod = ChaosMod::Get();

	if (mod)
	{
		ChaosMod::LogToFile("Stopping WebSockets server (F11)");

		ChaosMod::globalMutex.lock();

		if (mod->wsServer)
		{
			MessageBeep(MB_OK);
			mod->wsServer->Stop();
			delete mod->wsServer;
			mod->wsServer = nullptr;

		}

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mod->NodeProcessInformation.dwProcessId);

		if (hProcess != NULL)
		{
			TerminateProcess(hProcess, 0);

			CloseHandle(hProcess);
		}

		ChaosMod::globalMutex.unlock();
	}
}

void ChaosMod::Main()
{
	InitWeaponHashes();
	InitWeatherHashes();
	InitEffects();

	ChaosMod::globalMutex.lock();

	ChaosMod::LastTick = GetTickCount();

	ChaosMod::globalMutex.unlock();

	ChaosMod::ThreadID = GetCurrentThreadId();

	ChaosMod::UpdatePlayerSkinHash();

	config.Read();

	std::string logStr = "Chaos Mod Loaded. Twitch: " + std::to_string(config.bTwitch);

	LogToFile(logStr.c_str());

	if (config.bTwitch)
	{
		StartWSServer();
	}

	while (true)
	{
		Update();
		InputTick();
		WAIT(0);
	}

}

void ChaosMod::Update()
{
	if (GetTickCount() < notification2Data.hideTime)
	{
		RenderNotification2();
	}

	ChaosMod::globalMutex.lock();

	uint32_t thisTick = GetTickCount();

	ChaosMod::_DeltaTime = thisTick - ChaosMod::LastTick;
	ChaosMod::_DeltaTimeSeconds = float(float(ChaosMod::_DeltaTime) / 1000.0f);

	ChaosMod::LastTick = thisTick;
	ChaosMod::PLAYER_PED = PLAYER::PLAYER_PED_ID();

	ChaosMod::globalMutex.unlock();

	if (!bEnabled)
	{
		return;
	}

	for (int32_t i = 0; i < activeEffects.size(); i++)
	{
		auto* effect = activeEffects[i];

		if (effect && effect->bTimed)
		{
			if (GetTickCount() >= effect->DeactivationTime)
			{
				effect->OnDeactivate();
				activeEffects.erase(activeEffects.begin() + i);
				i--;

				if (effect->bIsMeta)
				{
					ResetMetaTimer();
				}
			}
			else
			{
				effect->OnTick();
			}
		}
	}

	if (timeoutVotingStartTime && !bVotingEnabled && GetTickCount() >= timeoutVotingStartTime)
	{
		if (config.bTwitch && wsServer)
		{
			ChaosMod::globalMutex.lock();

			if (oldEffects.size() >= 12)
			{
				oldEffects.erase(oldEffects.begin(), oldEffects.begin() + 4);
			}

			pollEffects = GenerateEffectsWithChances(4);

			std::vector<std::string> effectNames;

			for (auto effect : pollEffects)
			{
				effectNames.push_back(effect->name);
				oldEffects.push_back(effect);
			}

			effectNames[effectNames.size() - 1] = "Random Effect";

			wsServer->SendEffectNamesToClient(effectNames);

			ChaosMod::globalMutex.unlock();
		}

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

		if (bEffectsActivatesAfterTimer)
		{
			if (config.bTwitch && wsServer)
			{
				ChaosMod::globalMutex.lock();

				wsServer->SendMessageToClient("vote_ended");

				ChaosMod::globalMutex.unlock();
			}
			else
			{
				int maxEffects = 1;
				if (activeMeta && activeMeta->ID == "combo_time")
				{
					maxEffects = 4;
				}

				auto effects = GenerateEffectsWithChances(maxEffects);
				for (auto* effect : effects)
				{
					ActivateEffect(effect);
				}

				if (effects[0])
				{
					prevActivatedEffect = effects[0];
				}
			}
		}
	}

	if (MetaActivationTime != 0 && GetTickCount() >= MetaActivationTime)
	{
		MetaActivationTime = 0;

		std::vector<MetaEffect*> enabledMetaEffects;

		for (auto* effect : AllMetaEffects)
		{
			if (effect->bEnabled)
			{
				enabledMetaEffects.push_back(effect);
			}
		}

		if (enabledMetaEffects.size() != 0)
		{
			MetaEffect* effect = enabledMetaEffects[rand() % enabledMetaEffects.size()];

			ActivateEffect(effect);
			activeMeta = effect;

			if (effect->ID == "total_chaos" && !bVotingEnabled)
			{
				ResetEffectsTimeout();
			}
		}
	}

	ChaosMod::globalMutex.lock();

	if (twitchWinnerID != -1)
	{
		if (twitchWinnerID < pollEffects.size())
		{
			auto* effect = pollEffects[twitchWinnerID];
			ActivateEffect(effect);
			prevActivatedEffect = effect;

			pollEffects.clear();

			if (activeMeta && activeMeta->ID == "combo_time")
			{
				auto comboEffects = GenerateEffectsWithChances(3);
				for (auto* effect : comboEffects)
				{
					ActivateEffect(effect);
				}
			}
		}

		twitchWinnerID = -1;
	}

	ChaosMod::globalMutex.unlock();

	DrawUI();
}

void ChaosMod::InputTick()
{
	if (isKeyPressed(VK_F7))
	{
		ToggleModStatus();
	}

	if (IsModEnabled())
	{
		if (isKeyPressed(VK_F8))
		{
			ToggleDefaultEffectActivation();
		}

		if (isKeyPressed(VK_F10))
		{
			ToggleEffectSelection();
		}

		if (isKeyPressed(VK_LEFT))
		{
			ChangeSelectedEffect(-1);

			if (modMenu.IsVisible())
			{
				modMenu.OnKeyPressed(VK_LEFT);
			}
		}

		if (isKeyPressed(VK_RIGHT))
		{
			ChangeSelectedEffect(1);

			if (modMenu.IsVisible())
			{
				modMenu.OnKeyPressed(VK_RIGHT);
			}
		}

		if (isKeyPressed(VK_DOWN))
		{
			if (modMenu.IsVisible())
			{
				modMenu.OnKeyPressed(VK_DOWN);
			}
		}

		if (isKeyPressed(VK_UP))
		{
			if (modMenu.IsVisible())
			{
				modMenu.OnKeyPressed(VK_UP);
			}
		}

		if (isKeyPressed(VK_RETURN))
		{
			ActivateSelectedEffect();
			
			if (modMenu.IsVisible())
			{
				modMenu.OnKeyPressed(VK_RETURN);
			}
		}

		//if (isKeyPressed(VK_F12))
		//{
			//modMenu.ToggleVisibility();
		//}

		if (isKeyPressed(VK_F12))
		{
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			if (ENTITY::DOES_ENTITY_EXIST(playerPed))
			{
				ENTITY::SET_ENTITY_INVINCIBLE(playerPed, false);
				ENTITY::SET_ENTITY_HEALTH(playerPed, 0, 0);
			}
		}
	}

	/** 
	 * For unknown reason, in debug mode mod breaks when trying to disable it with Ctrl+R if websocketpp loop is active
	 * So you need to press F11 first to stop the websocket server and terminate its thread
	*/
	if (isKeyPressed(VK_F11))
	{
		ChaosMod::StopServer();
	}
}

void ChaosMod::DrawUI()
{
	/** 18px on 1080p */
	static const float ProgressBarHeight = 18.0f / 1080.f;

	/** Draw progress bar background */
	GRAPHICS::DRAW_RECT(0.5f, ProgressBarHeight / 2.0f, 1.0f, ProgressBarHeight, 0, 0, 0, 168, 0, 0);

	float effectTimeoutValue = float(float(GetTickCount() - timeoutStartTime) / float(timeoutEndTime - timeoutStartTime));

	if (effectTimeoutValue < 0.0f) effectTimeoutValue = 0.0f;
	else if (effectTimeoutValue > 1.0f) effectTimeoutValue = 1.0f;

	LinearColor color(143, 6, 6, 255);

	if (activeMeta && activeMeta->ID == "total_chaos")
	{
		LinearColor startColor(143, 6, 6, 240);
		LinearColor endColor(249, 200, 4, 240);

		float alpha = ((sin(metaEffectColorSinX) + 1.0f) / 2.0f);

		color.R = Lerp(startColor.R, endColor.R, alpha);
		color.G = Lerp(startColor.G, endColor.G, alpha);
		color.B = Lerp(startColor.B, endColor.B, alpha);
	}

	/** Draw progress bar foreground */
	GRAPHICS::DRAW_RECT(effectTimeoutValue / 2.0f, ProgressBarHeight / 2.0f, effectTimeoutValue, ProgressBarHeight, color.R, color.G, color.B, color.A, 0, 0);

	for (int32_t i = 0; i < activeEffects.size(); i++)
	{
		DrawEffectInUI(activeEffects[i], i);
	}

	if (bEffectSelectionVisible)
	{
		auto* Effect = AllEffects[debugSelectedEffectIndex];

		std::string debugText = "<font face='$title'>";
		debugText += "[DEBUG] Effect to activate: \n[" + std::to_string(debugSelectedEffectIndex+1) + "] " + Effect->name;
		debugText += "</font>";

		UI::SET_TEXT_SCALE(0.0f, 0.5f);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 240);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 255);

		char* varString = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)debugText.c_str());
		varString = invoke<char*>(0xFA925AC00EB830B9, 42, (char*)"COLOR_STRING", 0, varString);

		UI::DRAW_TEXT(varString, 0.5f, 0.5f);
	}

	if (modMenu.IsVisible())
	{
		modMenu.Draw();
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

	int _effectsInterval = effectsInterval;

	bool bTotalChaos = activeMeta && activeMeta->ID == "total_chaos";

	if (bTotalChaos)
	{
		_effectsInterval = 15;
	}

	timeoutEndTime = timeoutStartTime + (_effectsInterval * 1000);

	int _effectsVoteTime = 0;

	if (config.bTwitch)
	{
		_effectsVoteTime = effectsVoteTime;
	}
	else
	{
		_effectsVoteTime = _effectsInterval / 2;
	}

	if (bTotalChaos)
	{
		_effectsVoteTime = _effectsInterval - 2;
	}

	timeoutVotingStartTime = timeoutEndTime - (_effectsVoteTime * 1000);
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

	if (effect->bTimed && !effect->bIsFake)
	{
		float progressBarValue = 1.0f - (((float)GetTickCount() - (float)effect->ActivationTime) / ((float)effect->DeactivationTime - (float)effect->ActivationTime));
		float progressBarWidth = (400.0f / 1920.0f);
		float progressBarX = X - (((1.0f - (progressBarValue)) / 2.0f) * progressBarWidth);
		GRAPHICS::DRAW_RECT(progressBarX, Y, (progressBarWidth) * progressBarValue, 40.0f / 1080.0f, 0, 0, 0, 185, 0, 0);
	}

	UI::SET_TEXT_SCALE(0.0f, 0.35f);

	if (effect->bIsMeta)
	{
		LinearColor startColor(239, 86, 86, 240);

		LinearColor endColor(249, 200, 4, 240);

		metaEffectColorSinX += ChaosMod::GetDeltaTimeSeconds() * 5.0f;

		float alpha = ((sin(metaEffectColorSinX) + 1.0f) / 2.0f);

		LinearColor color;
		
		color.R = Lerp(startColor.R, endColor.R, alpha);
		color.G = Lerp(startColor.G, endColor.G, alpha);
		color.B = Lerp(startColor.B, endColor.B, alpha);
		
		UI::SET_TEXT_COLOR_RGBA(color.R, color.G, color.B, 240);
	}
	else
	{
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 240);
	}

	UI::SET_TEXT_CENTRE(1);
	UI::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 255);

	std::string effectName = "<font face='$title'>";
	
	if (effect->bIsMeta)
	{
		effectName += "[META] ";
	}
	
	effectName += effect->name + "</font>";
		
	char* varString = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)effectName.c_str());

	varString = invoke<char*>(0xFA925AC00EB830B9, 42, (char*)"COLOR_STRING", 0, varString);

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
		new EffectSpawnHotchkissCannon(),
		new EffectKidnapping(),
		new EffectSpawnHorse(),
		new EffectTeleportEverything(),
		new EffectSpawnWagon(),
		new EffectSpawnMule(),
		new EffectSpawnDonkey(),
		new EffectSpawnSerialKiller(),
		new EffectToTheStars(),
		new EffectSnowstorm(),
		new EffectThunderstorm(),
		new EffectTeleportToDesert(),
		new EffectTeleportToValentine(),
		new EffectTeleportToSaintDenis(),
		new EffectTeleportToSnowyMountains(),
		new EffectTeleportToPrison(),
		new EffectGivePlayerMoney(),
		new EffectBankruptcy(),
		new EffectSpawnVampire(),
		new EffectGiveRifle(),
		new EffectGiveRevolver(),
		new EffectRemoveAllWeapons(),
		new EffectNoAmmo(),
		new EffectDropWeapon(),
		new EffectHealPlayer(),
		new EffectAlmostDead(),
		new EffectRestoreStamina(),
		new EffectFlipAllVehs(),
		new EffectGiantPeds(),
		new EffectAllPedsWannaKillPlayer(),
		new EffectSpawnMiniDonkey(),
		new EffectSpawnGiantDonkey(),
		new EffectRagdoll(),
		new EffectRagdollEveryone(),
		new EffectSpawnGiantCop(),
		new EffectLaunchPedsUp(),
		new EffectInvertedGravity(),
		new EffectDoomsday(),
		new EffectGiveRandomWeapon(),
		new EffectSetDrunk(),
		new EffectSpawnAngryDwarf(),
		new EffectSpawnAngrySkeleton(),
		new EffectSpawnCompanionBertram(),
		new EffectPlayIntro(),
		new EffectSetRandomWeather(),
		new EffectSetSunnyWeather(),
		new EffectSetRainyWeather(),
		new EffectSetRapidWeather(),
		new EffectSetTimeMorning(),
		new EffectSetTimeNight(),
		new EffectSetRandomTime(),
		new EffectClearPursuit(),
		new EffectIncreaseBounty(),
		new EffectSpawnFrozenCouple(),
		new EffectEarthquake(),
		new EffectRemoveCurrentVehicle(),
		new EffectMinecartRain(),
		new EffectGiveLasso(),
		new EffectFullAcceleration(),
		new EffectEveryoneIsInvincible(),
		new EffectTeleportToVanHorn(),
		new EffectEveryoneExitsVehs(),
		new EffectSetPlayerIntoRandomVeh(),
		new Effect120FOV(),
		new EffectSpawnRobot(),
		new EffectSpawnHotAirBalloon(),
		new EffectSpawnLassoGuy(),
		new EffectIgnitePlayersWagon(),
		new EffectIgniteNearbyWagons(),
		new EffectIgnitePlayer(),
		new EffectIgniteNearbyPeds(),
		new EffectLightningOnce(),
		new EffectKickflip(),
		new EffectSpawnCanoe(),
		new EffectHorsesRain(),
		new EffectLightningEnemy(),
		new EffectLightningWeapons(),
		new EffectSkyrimIntro(),
		new EffectCowSkin(),
		new EffectDwarfSkin(),
		new EffectPigSkin(),
		new EffectRatSkin(),
		new EffectTurtleSkin(),
		new EffectSpawnParrotCompanion(),
		new EffectHonorGood(),
		new EffectHonorBad(),
		new EffectHonorReset(),
		new EffectSpawnShireHorse(),
		new EffectAltTab(),
		new EffectPlayerSleep(),
		new EffectPlayerIsMinion(),
		new EffectTeleportToWaypoint(),
		new EffectRemoveWeaponFromEveryone(),
		new EffectUndeadNightmare(),
		new EffectBloodTrails(),
		new EffectDetachWheels(),
		new EffectExplosiveWeapons(),
		new EffectSetFoggyWeather(),
		new EffectGhostTown(),
		new EffectSpawnDogCompanion(),
		new EffectSpawnCatCompanion(),
		new EffectSetPedsIntoPlayerVehicle(),
		new EffectSetRandomWalkStyle(),
		new EffectTeleportWeapons(),
		new EffectGiveSniperRifle(),
		new EffectGiveDynamite(),
		new EffectThrowingKnives(),
		new EffectSpawnBearCompanion(),
		new EffectTeleportFewMeters(),
		new EffectBlackingOut(),
		new EffectRandomClothes(),
		new EffectSpawnAngryCorpse(),
		new EffectFastPlayersWagon(),
		new EffectSpawnAngryCaveman(),
		new EffectSpawnAngryTwin(),
		new EffectMostWanted(),
		new EffectSpawnUFO(),
		new EffectGravityField(),
		new EffectPigWeapons(),
		new EffectSpawnAngryCowboy(),
		new EffectSpawnUndeadBoss(),
		new EffectSpawnGrieferMicah(),
		new EffectTeleportToLake(),
		new EffectSetWinterOutfit(),
		new EffectGamespeedx02(),
		new EffectGamespeedx05(),
		new EffectSuperJump(),
		new EffectDisableLeftRight(),
		new EffectDisableForwardBackward(),
		new EffectDisableAllMovements(),
		new EffectDisableSprintJump(),
		new EffectDisableAttackButton(),
		new EffectDisableAiming(),
		new EffectWaterfall(),
		new EffectRandomWheelsDetaching(),
		new EffectRainingPigs(),
		new EffectRandomHonor(),
		new EffectDutchStealsPlayersVeh(),
		new EffectSpawnPredator(),
		new EffectInvertVelocity(),
		new EffectIncreaseVelocity(),
		new EffectBunnyhop(),
		new EffectEyeDisorder(),
		new EffectPedsBhop(),
		new EffectPedsSpin(),
		new EffectBirdSkin(),
		new EffectBodySwap(),
		new EffectCloneEnemy(),
		new EffectPedsFollowPlayer(),
		new EffectPedsFleeing(),
		new EffectPlayerSpin(),
		new EffectRainbow(),
		new EffectPlayerLosesWeight(),
		new EffectPlayerGainsWeight(),
		new EffectSetRandomHat(),
		new EffectHealNearbyPeds(),
		new EffectReviveDeadPeds(),
		new EffectGravityGun(),
		new EffectSpawnAngryTommy(),
		new EffectDisableDeadEye(),
		new EffectPartyTime(),
		new EffectSetRandomVelocity(),
		new EffectTeleportToFortWallace(),
		new EffectTeleportToFortMercer(),
		new EffectTeleportToBlackwater(),
		new EffectExplodeNearbyPeds(),
		new EffectNearbyPedIsCompanion(),
		new EffectEveryoneRagdollsWhenShot(),
		new EffectNearbyPedIsEnemy(),
		new EffectExplosiveCombat(),
		new EffectGiveEveryoneRifle(),
		new EffectTimelapse(),
		new EffectNoHUD(),
		new EffectFirstPerson(),
		new EffectTopDownCamera(),
		new EffectAgitateHorse(),
		new EffectODriscolls(),
		new EffectInsaneGravity(),
		new EffectUpsideDownCamera(),
		new EffectOneHitKO(),
		new EffectBanditoKidnapsPlayer(),
		new EffectTpRandomLocation(),
		new EffectFakeTeleport(),
		new EffectSpawnRandomVeh(),
		new EffectHorsesAreDonkeys()
	};

	EffectsMap.clear();

	for (auto* effect : AllEffects)
	{
		EffectsMap.emplace(effect->ID, effect);
	}

	AllMetaEffects = {
		new MetaEffectTotalChaos(),
		new MetaEffectComboTime()
	};

	MetaEffectsMap.clear();

	for (auto* effect : AllMetaEffects)
	{
		MetaEffectsMap.emplace(effect->ID, effect);
	}
}

void ChaosMod::ToggleDefaultEffectActivation()
{
	MessageBeep(MB_OK);

	bEffectsActivatesAfterTimer = !bEffectsActivatesAfterTimer;
}

void ChaosMod::ToggleEffectSelection()
{
	bEffectSelectionVisible = !bEffectSelectionVisible;
}

void ChaosMod::ChangeSelectedEffect(int value)
{
	if (debugSelectedEffectIndex == 0 && value == -1)
	{
		debugSelectedEffectIndex = AllEffects.size() - 1;
		AUDIO::PLAY_SOUND_FRONTEND((char*)"NAV_LEFT", (char*)"Ledger_Sounds", true, 0);
	}
	else
	{
		debugSelectedEffectIndex += value;

		if (debugSelectedEffectIndex >= AllEffects.size())
		{
			debugSelectedEffectIndex = 0;
		}

		AUDIO::PLAY_SOUND_FRONTEND((char*)"NAV_RIGHT", (char*)"Ledger_Sounds", true, 0);

	}
}

void ChaosMod::ActivateSelectedEffect()
{
	if (bEffectSelectionVisible)
	{
		auto* effect = AllEffects[debugSelectedEffectIndex];

		if (effect)
		{
			ActivateEffect(effect);
		}

		bEffectSelectionVisible = false;

		AUDIO::PLAY_SOUND_FRONTEND((char*)"INFO", (char*)"HUD_SHOP_SOUNDSET", true, 0);
	}
}

void ChaosMod::ShowNotification(const char* title, const char* subtitle, const char* iconDict, const char* iconName, int32_t durationMs, const char* iconColor)
{
	if (!notificationDuration)
	{
		notificationDuration = new NotificationDuration();
	}

	if (!notificationData)
	{
		notificationData = new NotificationData();
	}

	notificationDuration->ms = durationMs;

	if (strlen(iconColor) == 0)
	{
		iconColor = "COLOR_PURE_WHITE";
	}

	notificationData->title = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)title);
	notificationData->subtitle = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)subtitle);

	notificationData->iconDict = GAMEPLAY::GET_HASH_KEY((char*)iconDict);
	notificationData->icon = GAMEPLAY::GET_HASH_KEY((char*)iconName);
	notificationData->iconColor = GAMEPLAY::GET_HASH_KEY((char*)iconColor);

	if (!TEXTURE::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)iconDict))
	{
		TEXTURE::REQUEST_STREAMED_TEXTURE_DICT((char*)iconDict, 0);
		WAIT(1000);
	}

	/** _UI_FEED_POST_SAMPLE_TOAST */
	UIUNK::_0x26E87218390E6729((Any*)notificationDuration, (Any*)notificationData, 1, 1);

	TEXTURE::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED((char*)iconDict);
}

void ChaosMod::ShowNotification2(const char* title, const char* subtitle, uint32_t durationMs, const char* iconDict, const char* iconName, LinearColor iconColor)
{
	notification2Data.bSubtitleExists = strlen(subtitle) != 0;
	notification2Data.title = "<font face='$title'>";
	notification2Data.title += title;
	notification2Data.title += "</font>";

	notification2Data.subtitle = "<font face='$body2'>";
	notification2Data.subtitle += subtitle;
	notification2Data.subtitle += "</font>";

	notification2Data.hideTime = GetTickCount() + durationMs;
	notification2Data.showTime = GetTickCount();

	if (!TEXTURE::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)iconDict))
	{
		TEXTURE::REQUEST_STREAMED_TEXTURE_DICT((char*)iconDict, 0);
		WAIT(100);
	}

	if (!TEXTURE::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)"feeds"))
	{
		TEXTURE::REQUEST_STREAMED_TEXTURE_DICT((char*)"feeds", 0);
		WAIT(100);
	}

	notification2Data.icon.dict = iconDict;
	notification2Data.icon.name = iconName;
	notification2Data.iconColor = iconColor;

}

void ChaosMod::RenderNotification2()
{
	float yOffset = 0.35f;

	float opacity = 1.0f;
	uint32_t timeNow = GetTickCount();

	const uint32_t fadeTime = 300;

	if (timeNow < notification2Data.showTime + fadeTime)
	{
		opacity = float(timeNow - notification2Data.showTime) / float(notification2Data.showTime + fadeTime - notification2Data.showTime);
	}
	else if (timeNow > notification2Data.hideTime - fadeTime)
	{
		uint32_t start = notification2Data.hideTime - fadeTime;
		opacity = 1.0f-(float(timeNow - start) / float(notification2Data.hideTime - start));
	}

	if (opacity < 0.0f)
	{
		opacity = 0.0f;
	}
	else if (opacity > 1.0f)
	{
		opacity = 1.0f;
	}

	uint8_t alpha = uint8_t(255.0f * opacity);

	if (!TEXTURE::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)notification2Data.icon.dict))
	{
		TEXTURE::REQUEST_STREAMED_TEXTURE_DICT((char*)notification2Data.icon.dict, 0);
		WAIT(100);
	}

	if (!TEXTURE::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)"feeds"))
	{
		TEXTURE::REQUEST_STREAMED_TEXTURE_DICT((char*)"feeds", 0);
		WAIT(100);
	}

	GRAPHICS::DRAW_SPRITE((char*)"feeds", (char*)"help_text_bg", 0.5f, 0.5f + yOffset, 512.0f / 1920.0f, 128.0f / 1080.0f, 0.0f, 0, 0, 0, alpha, false);

	/** Icon color */
	LinearColor iconC = notification2Data.iconColor;
	iconC.A = uint8_t(float(iconC.A) * opacity);

	GRAPHICS::DRAW_SPRITE((char*)notification2Data.icon.dict, (char*)notification2Data.icon.name, 0.41f, 0.5f + yOffset, 0.05f, 96.0f / 1080.0f, 0.0f, iconC.R, iconC.G, iconC.B, iconC.A, false);

	UI::SET_TEXT_SCALE(0.0f, 0.5f);
	UI::SET_TEXT_COLOR_RGBA(255, 255, 255, alpha);
	UI::SET_TEXT_CENTRE(false);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	
	float textY = 0.5f;

	if (notification2Data.bSubtitleExists)
	{
		textY = 0.46f;
	}
	else
	{
		textY = 0.48f;
	}

	notification2Data.__titleVarStr = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)notification2Data.title.c_str());
	notification2Data.__titleVarStr = invoke<const char*>(0xFA925AC00EB830B9, 42, (char*)"COLOR_STRING", 0, notification2Data.__titleVarStr);

	UI::DRAW_TEXT((char*)notification2Data.__titleVarStr, 0.44f, textY + yOffset);

	if (notification2Data.bSubtitleExists)
	{
		UI::SET_TEXT_SCALE(0.0f, 0.4f);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, alpha);
		UI::SET_TEXT_CENTRE(false);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);

		auto subtitle = GAMEPLAY::CREATE_STRING(10, (char*)"LITERAL_STRING", (char*)notification2Data.subtitle.c_str());
		subtitle = invoke<char*>(0xFA925AC00EB830B9, 42, (char*)"COLOR_STRING", 0, subtitle);

		UI::DRAW_TEXT((char*)subtitle, 0.44f, 0.5f + yOffset);
	}
}

void ChaosMod::UpdatePlayerSkinHash()
{
	static Hash playerSkinArthur = GAMEPLAY::GET_HASH_KEY((char*)"Player_Zero");
	static Hash playerSkinJohn = GAMEPLAY::GET_HASH_KEY((char*)"Player_Three");

	uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;

	if (*ptr1 == playerSkinArthur || *ptr1 == playerSkinJohn)
	{
		ChaosMod::PlayerSkin1 = *ptr1;
		ChaosMod::PlayerSkin2 = *ptr2;
	}
}

void ChaosMod::ResetPlayerSkin()
{
	Hash currentSkin = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());

	static Hash playerSkinArthur = GAMEPLAY::GET_HASH_KEY((char*)"Player_Zero");
	static Hash playerSkinJohn = GAMEPLAY::GET_HASH_KEY((char*)"Player_Three");

	if (currentSkin == playerSkinArthur || currentSkin == playerSkinJohn)
	{
		return;
	}

	LoadModel(ChaosMod::PlayerSkin1);

	PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), ChaosMod::PlayerSkin1, 1);
	*getGlobalPtr(0x23) = PLAYER::PLAYER_PED_ID();

	uint64_t* ptr1 = getGlobalPtr(0x28) + 0x27;
	uint64_t* ptr2 = getGlobalPtr(0x1D890E) + 2;

	*ptr1 = ChaosMod::PlayerSkin1;
	*ptr2 = ChaosMod::PlayerSkin2;
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	ENTITY::SET_ENTITY_COLLISION(playerPed, true, true);
	ENTITY::SET_ENTITY_DYNAMIC(playerPed, true);
}

void ChaosMod::StartWSServer()
{
	wsServer = new WebSocketServer();
	wsServer->Init(9149);

	LogToFile("Creating WebSockets thread..");

	wsThread = std::thread([this] {  this->wsServer->Run(); });

	wsThread.detach();

	TerminateNodeProcess();

	StartNodeProcess();
}

std::vector<Effect*> ChaosMod::GenerateEffectsWithChances(uint32_t maxEffects)
{
	std::vector<Effect*> effects;
	std::vector<uint16_t> effectsIndices = {};
	std::set<std::string> oldEffectIDs;

	for (auto oldEffect : oldEffects)
	{
		oldEffectIDs.insert(oldEffect->ID);
	}

	for (uint32_t i = 0; i < config.effects.size(); i++)
	{
		ConfigEffect& configEffect = config.effects[i];

		/**
		 * Skip this effect if it was disabled by user or activated in the previous time 
		 */
		if (!configEffect.bEnabled || oldEffectIDs.contains(configEffect.id) || (prevActivatedEffect && prevActivatedEffect->ID == configEffect.id))
		{
			continue;
		}

		configEffect.chance = configEffect.chance < 1 ? 1 : (configEffect.chance > 10 ? 10 : configEffect.chance);

		effectsIndices.insert(effectsIndices.end(), configEffect.chance, i);
	}

	for (uint32_t i = 0; i < maxEffects; i++)
	{
		uint32_t randomIndex = rand() % effectsIndices.size();
		uint16_t effectIndex = effectsIndices[randomIndex];

		ConfigEffect configEffect = config.effects[effectIndex];

		auto* effect = EffectsMap[configEffect.id];

		if (effect)
		{
			effects.push_back(effect);
		}

		if (maxEffects == 1)
		{
			break;
		}

		/**
		 * Find a first index of the effect in effectsIndices list, then delete all of this effect's indices
		 * Effects will not be repeated in the output array if we do this
		 */
		uint32_t effectIndexStart = randomIndex;

		for (int i = randomIndex; i >= 0; i--)
		{
			if (effectsIndices[i] == effectIndex)
			{
				effectIndexStart = i;
			}
		}
		effectsIndices.erase(effectsIndices.begin() + effectIndexStart, effectsIndices.begin() + (effectIndexStart + configEffect.chance) );
	}

	return effects;
}

void ChaosMod::LogToFile(const char* str)
{
	if (!ChaosMod::Singleton || !ChaosMod::Singleton->config.bTwitch)
	{
		return;
	}

	ChaosMod::globalMutex.lock();

	static auto logFilePath = std::filesystem::current_path() / L"ChaosMod" / L"chaos.log";

	ChaosMod::logString += str;
	ChaosMod::logString += "\n";

	std::ofstream outFile(logFilePath);
	outFile << logString;
	outFile.close();

	ChaosMod::globalMutex.unlock();
}

void ChaosMod::ResetMetaTimer()
{
	if (config.metaInterval == 0)
	{
		MetaActivationTime = 0;
	}
	else
	{
		MetaActivationTime = GetTickCount() + (config.metaInterval * 1000);
	}

	activeMeta = nullptr;
	metaEffectColorSinX = 0.0f;
}