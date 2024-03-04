#include <SKSE/GameRTTI.h>

#include "Papyrus.h"
#include "ConfigStore.h"
#include "Relocation.h"
#include "InterfaceManager.h"
#include "IOManager.h"

namespace Papyrus
{
	void ToggleInterface(StaticFunctionTag* base)
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		data->modActive = !data->modActive;
		Relocation::MCM_SetModSettingFloat(base, "MinimalMinimap", "fModActive:General", static_cast<float>(data->modActive));
		InterfaceManager::UpdateConfig();
	}

	void ToggleZoom(StaticFunctionTag* base)
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		data->nowZoomLevel += 1;
		if (data->nowZoomLevel > data->maxZoomLevel)
		{
			data->nowZoomLevel = 0;
		}
		Relocation::MCM_SetModSettingFloat(base, "MinimalMinimap", "fCustomZoomLevel:General", data->nowZoomLevel);
		InterfaceManager::UpdateConfig();
	}

	bool RegisterFuncs(VirtualMachine* vm)
	{
		_MESSAGE("Functions Registered");

		vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("ToggleInterface", "MinimalMinimapMCM", ToggleInterface, vm));

		vm->RegisterFunction(new NativeFunction0<StaticFunctionTag, void>("ToggleZoom", "MinimalMinimapMCM", ToggleZoom, vm));

		return true;
	}
};
