#pragma once

#ifndef hConfigStore
#define hConfigStore

#include <set>
#include <vector>

#include <SKSE/GameReferences.h>
#include "DataStruct.h"

namespace ConfigStore
{
	class ConfigData
	{
	public:
		bool modActive = false;

		bool displaySymbol[DisplayType::_MAX];

		bool skipAtCombatSymbol[DisplayType::_MAX];

		bool rotateMinimap = false;
		bool checkLOS = false;
		bool useLocalMap = false;
		bool useCamRotation = false;
		bool playSoundEffect = false;
		bool drawObjectiveOver = false;
		bool customMarkerAsObj = false;
		bool paMarkerAsObj = false;
		bool ignoreHeightForOver = false;
		bool playerAffectZoom = false;

		float soundDelay = 1.0f;
		float localMapRect = 4000.0f;
		float radius = 3000.0f;
		float renderRadius = 135.0f;
		float targetFPS = 15.0f;
		float localUpdateDelay = 10.0f;
		float localMapMult = 6.0f;
		float targetStealthPoint = 0.0f;

		float interiorMapMult = 0.7f;

		float zoomMultList[4] = {};

		float nowZoomLevel = 0.0f;
		float maxZoomLevel = 3.0f;

		std::string soundFile = "";

		std::vector<double> interfaceConfigs;

		bool functionFound = false;
	};

	ConfigData* GetInstance();

	void Clear();

	bool GetMCMConfig();
}

#endif // !hConfigStore

