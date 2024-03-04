#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include <direct.h>

#include <SKSE/GameReferences.h>
#include <SKSE/GameData.h>
#include <SKSE/GameRTTI.h>

#include "Relocation.h"
#include "Utility.h"
#include "ConfigStore.h"
#include "InterfaceManager.h"

namespace IOManager
{
	std::string	modulePath = "";

	void GetMenuName()
	{
		std::vector<std::string>* lines = Utility::GetLinesFromFile(modulePath + "\\SKSE\\Plugins\\MM_UIName.txt");
		for (std::string line : *lines)
		{
			line = std::regex_replace(line, std::regex("^\\s+"), "");
			line = std::regex_replace(line, std::regex("\\s+$"), "");
			line = std::regex_replace(line, std::regex("\\.swf$"), "");
			if (!line.empty())
			{
				InterfaceManager::SetInterfaceName(line);
				break;
			}
		}
	}

	void LoadHideMenuNames()
	{
		std::vector<std::string> names;
		std::vector<std::string>* lines = Utility::GetLinesFromFile(modulePath + "\\SKSE\\Plugins\\MM_HideUI.txt");
		for (std::string line : *lines)
		{
			line = std::regex_replace(line, std::regex("^\\s+"), "");
			line = std::regex_replace(line, std::regex("\\s+$"), "");
			names.push_back(line);
		}
		InterfaceManager::InsertMenuName(&names);
		delete lines;
	}

	void SetStatic()
	{
		char resultBuf[2048];
		UInt32 tInt = GetModuleFileName(GetModuleHandle(NULL), resultBuf, sizeof(resultBuf));
		modulePath = std::string(resultBuf, tInt);
		modulePath = modulePath.substr(0, modulePath.find_last_of('\\')) + "\\Data";

		LoadHideMenuNames();
		GetMenuName();

		void* ptr = nullptr;

		ptr = Utility::GetFunctionProxy("MCM", "GetModSettingFloat");
		if (ptr == nullptr)
		{
			_MESSAGE("Get MCM::GetModSettingFloat Failed.");
			return;
		}
		Relocation::MCM_GetModSettingFloat = (Relocation::_MCM_GetModSettingFloat)ptr;

		ptr = Utility::GetFunctionProxy("MCM", "GetModSettingString");
		if (ptr == nullptr)
		{
			_MESSAGE("Get MCM::GetModSettingString Failed.");
			return;
		}
		Relocation::MCM_GetModSettingString = (Relocation::_MCM_GetModSettingString)ptr;

		ptr = Utility::GetFunctionProxy("MCM", "SetModSettingFloat");
		if (ptr == nullptr)
		{
			_MESSAGE("Get MCM::SetModSettingFloat Failed.");
			return;
		}
		Relocation::MCM_SetModSettingFloat = (Relocation::_MCM_SetModSettingFloat)ptr;

		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		data->functionFound = true;
	}
};
