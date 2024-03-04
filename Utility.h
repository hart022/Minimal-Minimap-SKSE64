#pragma once

#ifndef hUtility
#define hUtility

#include <vector>
#include <mutex>

#include <SKSE/GameReferences.h>
#include <SKSE/GameExtraData.h>
#include <SKSE/PluginAPI.h>

#include "DataStruct.h"

namespace Utility
{
	extern std::atomic<float> mapResult;
	extern std::atomic<bool> doUpdate;
	extern std::atomic<float> playerX;
	extern std::atomic<float> playerY;

	std::vector<std::string> split_string(std::string, char);

	std::vector<std::string>* GetLinesFromFile(std::string dataPath);

	std::string Base64Decode(std::string);
	std::string Base64Encode(std::string);

	UInt32 GetEquippedMask(Actor*);

	bool HasCustomName(ExtraTextDisplayData*);

	extern SKSETaskInterface* g_task;

	TESForm* GetFormByString(std::string str, char delimeter = '|');
	UInt32 GetIDByString(std::string str, char delimeter = '|');
	std::string GetStringByForm(TESForm* tForm, char delimeter = '|');
	std::string GetStringByID(UInt32 target, char delimeter = '|');

	bool CalcRealHostileToActor(Actor* from, Actor* to);
	void* GetFunctionProxy(const char* scope, const char* name);
	void RegisterLocalMapImage();

	void PlayOneShotWrapper();

	void CalcExtents_Pending(LocalMapRenderer* renderer, NiPoint3& lt, NiPoint3& rt, NiPoint3& lb);

	float GetNowRadius();
};

#endif