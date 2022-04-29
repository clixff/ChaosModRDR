#pragma once

#include <natives.h>
#include <types.h>
#include <enums.h>
#include <main.h>
#include <string>
#include <stdint.h>
#include <windows.h>
#include "Effects/effect.h"
#include <vector>
#include <map>
#include "server.h"
#include <thread>
#include <set>
#include "Misc/menu.h"
#include "Misc/config.h"

struct Vector2
{
	Vector2()
	{
	};
	
	Vector2(float _X, float _Y)
	{
		X = _X;
		Y = _Y;
	}
	
	float X = 0.0f;
	float Y = 0.0f;
};

struct KeyState
{
	uint32_t time = 0;
	bool bPressed = false;
};

struct NotificationDuration
{
	alignas(32) int32_t ms = 2000;
};

struct NotificationData
{
	alignas(8) int32_t __unknown = 0;
	alignas(8) const char* title;
	alignas(8) const char* subtitle;
	alignas(8) int32_t __unknown2 = 0;
	alignas(8) Hash iconDict;
	alignas(8) Hash icon;
	alignas(8) Hash iconColor = 0;
	alignas(8) int32_t __unknown3 = 0;
};

struct GameAssetData
{
	GameAssetData()
	{
	};
	
	GameAssetData(const char* _dict, const char* _name)
	{
		dict = _dict;
		name = _name;
	}
	
	const char* dict;
	const char* name;
};

struct Notification2Data
{
	std::string title;
	std::string subtitle;
	
	uint32_t hideTime = 0;
	uint32_t showTime = 0;
	
	GameAssetData icon;
	
	LinearColor iconColor;
	
	const char* __titleVarStr;
	const char* __subtitleVarStr;
	
	bool bSubtitleExists = false;
	
};


class ChaosMod
{
public:
	ChaosMod();
	
	~ChaosMod();
	
	static void ScriptMain();
	
	static ChaosMod* Singleton;
	
	/** Returns singleton */
	static ChaosMod* Get();
	
	bool IsModEnabled();
	
	static void
	DrawText(char* text, Vector2 position, Vector2 scale, LinearColor color, bool bCenter = true, int shadowSize = 0,
			 LinearColor shadowColor = LinearColor());
	
	static void
	OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore,
					  BOOL isUpNow);
	
	/** Enabled and disbles the mod */
	void ToggleModStatus();
	
	void ActivateEffect(Effect* effect);
	
	void ActivateSubEffect(int num_subs);
	
	void ActivateRandomEffect(int num_effects);
	
	bool IsEffectActive(std::string effect_id);
	
	WebSocketServer* wsServer = nullptr;
	
	std::thread wsThread;
	static std::mutex globalMutex;
	
	static void StopServer();
	
	void StartWSServer();
	
	static uint32_t LastTick;
	static uint64_t ThreadID;
	static Ped PLAYER_PED;
	static HMODULE hInstance;
	
	/** Set of peds created by mod */
	static std::set <Ped> pedsSet;
	/** Set of vehs created by mod */
	static std::set <Vehicle> vehsSet;
	/** Set of objects created by mod */
	static std::set <Entity> propsSet;
	
	
	static Hash PlayerSkin1;
	static Hash PlayerSkin2;
	
	static void UpdatePlayerSkinHash();
	
	static void ResetPlayerSkin();

private:
	void Main();
	
	void Update();
	
	void InputTick();
	
	void DrawUI();
	
	bool bEnabled = false;
	
	std::string ModUITextString;
	uint32_t ModUITextEndTime = 0;
	
	KeyState keyStates[0xFF] = {};
	
	bool isKeyPressed(uint8_t key);
	
	NotificationDuration* notificationDuration = nullptr;
	NotificationData* notificationData = nullptr;
	
	void ShowNotification(const char* title, const char* subtitle, const char* iconDict, const char* iconName,
						  int32_t durationMs = 2000, const char* iconColor = "");

public:
	/** Timers and effects */
	
	/** In seconds */
	int effectsInterval = 90;
	int effectsVoteTime = 45;
	
	std::vector<Effect*> AllEffects;
	std::map<std::string, Effect*> EffectsMap;

private:
	uint32_t timeoutStartTime = 0;
	uint32_t timeoutEndTime = 0;
	uint32_t timeoutVotingStartTime = 0;
	
	bool bVotingEnabled = false;
	
	void ResetEffectsTimeout();
	
	std::vector<Effect*> activeEffects;
	std::vector<Effect*> subEffects;
	std::vector<Effect*> displayedEffects;
	
	void DrawEffectInUI(Effect* effect, int32_t index);
	
	void InitEffects();

private:
	/** Debug */
	
	bool bEffectsActivatesAfterTimer = true;
	
	/** For debug only, disables effect activation after timer  */
	void ToggleDefaultEffectActivation();
	
	int32_t debugSelectedEffectIndex = 0;
	
	bool bEffectSelectionVisible = false;
	
	void ToggleEffectSelection();
	
	void ChangeSelectedEffect(int value);
	
	void ActivateSelectedEffect();

public:
	void StartNodeProcess();
	
	static void TerminateNodeProcess();
	
	STARTUPINFOW NodeStartupInfo;
	PROCESS_INFORMATION NodeProcessInformation;

private:
	
	/** Notifications 2 */
	Notification2Data notification2Data;
	
	void ShowNotification2(const char* title, const char* subtitle, uint32_t durationMs, const char* iconDict,
						   const char* iconName, LinearColor iconColor);
	
	void RenderNotification2();

private:
	ModMenu modMenu;
private:
	Config config;
public:
	/** This effect was activated in the previous time and will not be activated now */
	Effect* prevActivatedEffect = nullptr;
	
	std::vector<Effect*> GenerateEffectsWithChances(uint32_t maxEffects);
	
	/** Effects available for vote on Twitch chat now */
	std::vector<Effect*> pollEffects;

public:
	/**
	 * Index of winner effect in the Twitch poll
	 */
	int32_t twitchWinnerID = -1;
public:
	/** Difference between last and this tick in ms */
	static uint32_t _DeltaTime;
	/** Difference between last and this tick in seconds */
	static float _DeltaTimeSeconds;
	
	/** Get difference between last and this tick in ms */
	static inline uint32_t GetDeltaTime()
	{
		return ChaosMod::_DeltaTime;
	}
	
	/** Get difference between last and this tick in seconds */
	static inline float GetDeltaTimeSeconds()
	{
		return ChaosMod::_DeltaTimeSeconds;
	}
	
	static void LogToFile(const char* str);
	
	static std::string logString;

public:
	MetaEffect* activeMeta = nullptr;
	uint32_t MetaActivationTime = 0;
	std::vector<MetaEffect*> AllMetaEffects;
	
	std::map<std::string, MetaEffect*> MetaEffectsMap;
	
	void ResetMetaTimer();
	
	float metaEffectColorSinX = 0.0f;
public:
	std::vector<Effect*> oldEffects;
	
	std::string twitchViewerNameToSpawn = "";
	
	bool RequestTwitchViewerNameToSpawn();

public:
	void SavePlayerAttributes();
	
	void RestorePlayerAttributes();
	
	/** Health, stamina and dead eye ranks */
	int playerAttributesRanks[3];
	
	int playerDeadEyeLevel = 1;
	
	std::set <Hash> playerOldClothes;
};