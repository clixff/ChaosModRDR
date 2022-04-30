#include "effect.h"
#include "../script.h"

std::vector<const char*> Effect::WeaponNames = std::vector<const char*>();
std::vector <Hash> Effect::WeaponHashes = std::vector<Hash>();

std::vector<const char*> Effect::weatherNames = std::vector<const char*>();
std::vector <Hash> Effect::weatherHashes = std::vector<Hash>();

Effect::Effect()
{
	internalTimer = {};
}

Effect::~Effect()
{
}

void Effect::OnActivate()
{
	internalTimer = {};
}

void Effect::OnDeactivate()
{
	internalTimer = {};
}

void Effect::OnTick()
{
}

bool Effect::TimerTick(uint32_t maxMs)
{
	return TimerTick(maxMs, 0);
}

bool Effect::TimerTick(uint32_t maxMs, uint32_t timerIndex)
{
	internalTimer[timerIndex] += ChaosMod::GetDeltaTime();
	
	if (internalTimer[timerIndex] >= maxMs)
	{
		internalTimer[timerIndex] = internalTimer[timerIndex] % maxMs;
		return true;
	}
	
	return false;
}

void LoadModel(Hash model)
{
	//if (STREAMING::IS_MODEL_VALID(model))
	//{
	//	return;
	//}
	
	STREAMING::REQUEST_MODEL(model, true);
	
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
}

void InitWeaponHashes()
{
	Effect::WeaponNames = {"WEAPON_REPEATER_CARBINE", "WEAPON_REVOLVER_CATTLEMAN", "WEAPON_REVOLVER_CATTLEMAN_JOHN",
						   "WEAPON_REVOLVER_CATTLEMAN_MEXICAN", "WEAPON_REVOLVER_CATTLEMAN_PIG",
						   "WEAPON_REVOLVER_DOUBLEACTION_MICAH", "WEAPON_REVOLVER_DOUBLEACTION_GAMBLER",
						   "WEAPON_REVOLVER_DOUBLEACTION_EXOTIC", "WEAPON_REVOLVER_SCHOFIELD_CALLOWAY",
						   "WEAPON_REVOLVER_SCHOFIELD_GOLDEN", "WEAPON_REVOLVER_DOUBLEACTION",
						   "WEAPON_REVOLVER_SCHOFIELD", "WEAPON_REVOLVER_LEMAT", "WEAPON_PISTOL_VOLCANIC",
						   "WEAPON_PISTOL_M1899", "WEAPON_PISTOL_MAUSER", "WEAPON_PISTOL_MAUSER_DRUNK",
						   "WEAPON_PISTOL_SEMIAUTO", "WEAPON_REPEATER_WINCHESTER", "WEAPON_REPEATER_HENRY",
						   "WEAPON_RIFLE_VARMINT", "WEAPON_RIFLE_SPRINGFIELD", "WEAPON_RIFLE_BOLTACTION",
						   "WEAPON_SHOTGUN_DOUBLEBARREL", "WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC",
						   "WEAPON_SHOTGUN_SAWEDOFF", "WEAPON_SHOTGUN_REPEATING", "WEAPON_SHOTGUN_PUMP",
						   "WEAPON_SHOTGUN_SEMIAUTO", "WEAPON_SNIPERRIFLE_ROLLINGBLOCK",
						   "WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC", "WEAPON_SNIPERRIFLE_CARCANO", "WEAPON_MELEE_KNIFE",
						   "WEAPON_MELEE_KNIFE_JAWBONE", "WEAPON_MELEE_KNIFE_JOHN", "WEAPON_MELEE_KNIFE_MICAH",
						   "WEAPON_MELEE_KNIFE_MINER", "WEAPON_MELEE_KNIFE_VAMPIRE", "WEAPON_MELEE_KNIFE_CIVIL_WAR",
						   "WEAPON_MELEE_KNIFE_BEAR", "WEAPON_MELEE_BROKEN_SWORD", "WEAPON_MELEE_CLEAVER",
						   "WEAPON_MELEE_HATCHET", "WEAPON_MELEE_MACHETE", "WEAPON_MELEE_ANCIENT_HATCHET",
						   "WEAPON_MELEE_HATCHET_VIKING", "WEAPON_MELEE_HATCHET_HEWING", "WEAPON_MELEE_HATCHET_HUNTER",
						   "WEAPON_MELEE_HATCHET_HUNTER_RUSTED", "WEAPON_MELEE_HATCHET_DOUBLE_BIT",
						   "WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED", "WEAPON_THROWN_DYNAMITE", "WEAPON_THROWN_MOLOTOV",
						   "WEAPON_THROWN_THROWING_KNIVES", "WEAPON_THROWN_TOMAHAWK", "WEAPON_THROWN_TOMAHAWK_ANCIENT",
						   "WEAPON_REPEATER_EVANS",
			
			/** Should not be removed */
						   "WEAPON_BOW", "WEAPON_KIT_CAMERA", "WEAPON_LASSO", "WEAPON_FISHINGROD",
						   "WEAPON_MELEE_LANTERN_ELECTRIC"};
	
	Effect::WeaponHashes.clear();
	
	for (auto name : Effect::WeaponNames)
	{
		Effect::WeaponHashes.push_back(GAMEPLAY::GET_HASH_KEY((char*) name));
	}
}

void InitWeatherHashes()
{
	Effect::weatherNames = {"SUNNY", "MISTY", "FOG", "CLOUDS", "OVERCAST", "OVERCASTDARK", "DRIZZLE", "RAIN", "THUNDER",
							"THUNDERSTORM", "HURRICANE", "HIGHPRESSURE", "SHOWER", "HAIL", "SLEET", "SNOWCLEARING",
							"SNOWLIGHT", "SNOW", "BLIZZARD", "GROUNDBLIZZARD", "WHITEOUT", "SANDSTORM"};
	
	Effect::weatherHashes.clear();
	
	for (auto name : Effect::weatherNames)
	{
		Effect::weatherHashes.push_back(GAMEPLAY::GET_HASH_KEY((char*) name));
	}
}
