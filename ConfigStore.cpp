#include <SKSE/GameRTTI.h>

#include "ConfigStore.h"
#include "Relocation.h"
#include "InterfaceManager.h"

namespace ConfigStore
{
	ConfigData data;

	ConfigData* GetInstance()
	{
		return &data;
	}

	void Clear()
	{
		data.interfaceConfigs.clear();

		data.modActive = false;

		std::fill(data.displaySymbol, data.displaySymbol + DisplayType::_MAX, false);
		std::fill(data.skipAtCombatSymbol, data.skipAtCombatSymbol + DisplayType::_MAX, false);

		data.rotateMinimap = false;
		data.checkLOS = false;
		data.useLocalMap = false;
		data.useCamRotation = false;
		data.drawObjectiveOver = false;
		data.playerAffectZoom = false;
		data.customMarkerAsObj = false;
		data.paMarkerAsObj = false;
		data.ignoreHeightForOver = false;

		data.radius = 3000.0f;
		data.renderRadius = 135.0f;
		data.targetFPS = 15.0f;
		data.targetStealthPoint = 0.0f;
		data.localUpdateDelay = 10.0f;
		data.localMapMult = 6.0f;
		data.localMapRect = 4000.0f;

		data.playSoundEffect = false;
		data.soundDelay = 1.0f;
		data.soundFile = "";

		data.interiorMapMult = 0.7f;
		data.zoomMultList[0] = 0.7f;
		data.zoomMultList[1] = 0.5f;
		data.zoomMultList[2] = 0.3f;
		data.zoomMultList[3] = 1.0f;
		data.nowZoomLevel = 0.0f;
		data.maxZoomLevel = 3.0f;
	}

	bool GetMCMConfig()
	{
		if (data.functionFound)
		{
			const char* ModBaseName = "MinimalMinimap";
			StaticFunctionTag base;

			data.interfaceConfigs.clear();

			data.modActive = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fModActive:General") != 0.0f;
			data.rotateMinimap = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fRotateMinimap:General") != 0.0f;
			data.radius = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fRadius:General");
			data.targetFPS = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fTargetFPS:General");
			data.targetStealthPoint = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fTargetStealthPoint:General");
			data.checkLOS = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fCheckLOS:General") != 0.0f;
			data.localUpdateDelay = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fLocalUpdateDelay:General");
			data.useLocalMap = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fUseLocalMap:General") != 0.0f;
			data.localMapMult = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fLocalMapMult:General");
			data.useCamRotation = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fUseCamRot:General") != 0.0f;
			data.renderRadius = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fRenderRadius:General");
			data.drawObjectiveOver = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fDrawObjOverLine:General") != 0.0f;
			data.customMarkerAsObj = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fCusMarkerAsObj:General") != 0.0f;
			data.ignoreHeightForOver = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fIgnoreAltiOverObj:General") != 0.0f;
			data.paMarkerAsObj = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fPaMarkerAsObj:General") != 0.0f;

			data.playSoundEffect = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fPlaySound:General") != 0.0f;
			data.soundDelay = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fSoundDelay:General");
			data.soundFile = Relocation::MCM_GetModSettingString(&base, ModBaseName, "sSoundFile:General").c_str();

			data.localMapRect = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fLocalMapRect:General");

			data.interiorMapMult = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fInteriorMapZoom:General");
			data.zoomMultList[0] = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fFirstZoomLevel:General");
			data.zoomMultList[1] = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fSecondZoomLevel:General");
			data.zoomMultList[2] = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fThirdZoomLevel:General");
			data.zoomMultList[3] = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fFourthZoomLevel:General");
			data.nowZoomLevel = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fCustomZoomLevel:General");
			data.maxZoomLevel = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fMaxZoomLevel:General");
			data.playerAffectZoom = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fPlayerAffectZoom:General") != 0.0f;

			for (UInt32 i = 0; i < DisplayType::_MAX; i++)
			{
				if (DataStruct::DisplayTypeMap.count(i) != 0)
				{
					data.displaySymbol[i] = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, ("fDisplay" + DataStruct::DisplayTypeMap.at(i) + ":General").c_str()) != 0.0f;
					data.skipAtCombatSymbol[i] = Relocation::MCM_GetModSettingFloat(&base, ModBaseName, ("fSkipAtCombat" + DataStruct::DisplayTypeMap.at(i) + ":General").c_str()) != 0.0f;
				}
			}

			for (UInt32 i = 0; i < DisplayType::_MAX; i++)
			{
				if (DataStruct::DisplayTypeMap.count(i) != 0)
				{
					data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, ("fDisplay" + DataStruct::DisplayTypeMap.at(i) + "Type:Interface").c_str()));
				}
			}

			for (UInt32 i = 0; i < DisplayType::_MAX; i++)
			{
				if (DataStruct::DisplayTypeMap.count(i) != 0)
				{
					data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, ("fDisplay" + DataStruct::DisplayTypeMap.at(i) + "Opacity:Interface").c_str()));
				}
			}

			for (UInt32 i = 0; i < DisplayType::_MAX; i++)
			{
				if (DataStruct::DisplayTypeMap.count(i) != 0)
				{
					BSFixedString result = Relocation::MCM_GetModSettingString(&base, ModBaseName, ("sDisplay" + DataStruct::DisplayTypeMap.at(i) + "Color:Interface").c_str());
					if (result.c_str() != nullptr)
					{
						try
						{
							data.interfaceConfigs.push_back(static_cast<double>(std::stoul(result.c_str(), nullptr, 16)));
						}
						catch (const std::exception&)
						{
							//white
							data.interfaceConfigs.push_back(16777215.0);
						}
					}
					else
					{
						//white
						data.interfaceConfigs.push_back(16777215.0);
					}
				}
			}

			for (UInt32 i = 0; i < DisplayType::_MAX; i++)
			{
				if (DataStruct::DisplayTypeMap.count(i) != 0)
				{
					data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, ("fDisplay" + DataStruct::DisplayTypeMap.at(i) + "Priority:Interface").c_str()));
				}
			}

			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fAnchorType:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fXPosition:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fYPosition:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fInterfaceScale:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fSymbolScale:Interface"));

			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fRadarEnable:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fRadarAmount:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fBGOpacity:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fUseOffsetZ:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fOffsetZAmount:Interface"));

			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fBGGridType:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fBGGridRotate:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fBGGridAlpha:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fHideOffsetZ:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fInterfaceType:Interface"));

			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fLocalMapOpacity:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fFOVEnable:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fFOVOpacity:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fHideNIdicator:Interface"));

			try
			{
				std::vector<double> tempArr;

				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sBorderColor:Interface").c_str(), nullptr, 16)));
				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sBackgroundColor:Interface").c_str(), nullptr, 16)));
				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sGridColor:Interface").c_str(), nullptr, 16)));
				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sLocalMapColor:Interface").c_str(), nullptr, 16)));
				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sRadarColor:Interface").c_str(), nullptr, 16)));
				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sFOVColor:Interface").c_str(), nullptr, 16)));
				tempArr.push_back(static_cast<double>(std::stoul(Relocation::MCM_GetModSettingString(&base, ModBaseName, "sPlayerColor:Interface").c_str(), nullptr, 16)));

				data.interfaceConfigs.insert(data.interfaceConfigs.cend(), tempArr.begin(), tempArr.end());
			}
			catch (const std::exception&)
			{
				//white
				data.interfaceConfigs.push_back(16777215.0);
				data.interfaceConfigs.push_back(0.0);
				data.interfaceConfigs.push_back(16777215.0);
				data.interfaceConfigs.push_back(16777215.0);
				data.interfaceConfigs.push_back(43520.0);
				data.interfaceConfigs.push_back(26367.0);
				data.interfaceConfigs.push_back(16777215.0);
			}

			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fBorderOpacity:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fRadarOpacity:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fPlayerOpacity:Interface"));
			data.interfaceConfigs.push_back(Relocation::MCM_GetModSettingFloat(&base, ModBaseName, "fPlayerScale:Interface"));

			InterfaceManager::UpdateConfig();
		}
		return false;
	}
};
