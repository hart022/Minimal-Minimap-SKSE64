#include <ShlObj.h>

#include <SKSE/PluginManager.h>
#include <SKSE/GameMenus.h>

#include <SKSE_common/SKSE_version.h>

#include "Global.h"
#include "Trampoline.h"
#include "Papyrus.h"
#include "InterfaceManager.h"
#include "Utility.h"
#include "Message.h"
#include "Serialize.h"

/* Plugin Query */
extern "C" {

	bool SKSEPlugin_Query(const SKSEInterface* SKSE, PluginInfo* info) {
		std::string gLogPath{ "\\My Games\\Fallout4\\SKSE\\" PLUGIN_NAME ".log" };
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, gLogPath.c_str());
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME;
		info->version = PLUGIN_VERSION;


		if (SKSE->runtimeVersion != RUNTIME_VERSION_1_10_163) {
			_MESSAGE("unsupported runtime version %d", SKSE->runtimeVersion);
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface* SKSE) {
		_DMESSAGE("%s Loaded", PLUGIN_NAME);
		_DMESSAGE("ANSI(windows-1252) encoded");

		((SKSEMessagingInterface*)SKSE->QueryInterface(kInterface_Messaging))->RegisterListener(SKSE->GetPluginHandle(), "SKSE", Message::SKSEMessageHandler);
		((SKSEPapyrusInterface*)SKSE->QueryInterface(kInterface_Papyrus))->Register(Papyrus::RegisterFuncs);
		((SKSEScaleformInterface*)SKSE->QueryInterface(kInterface_Scaleform))->Register(PLUGIN_NAME, InterfaceManager::RegisterScaleform);
		Utility::g_task = (SKSETaskInterface*)SKSE->QueryInterface(kInterface_Task);

		if (!Trampoline::DoAlloc())
		{
			return false;
		}

		if (!Serialize::RegisterSerialize((SKSESerializationInterface*)SKSE->QueryInterface(kInterface_Serialization), SKSE->GetPluginHandle()))
		{
			return false;
		}

		return true;
	}
};