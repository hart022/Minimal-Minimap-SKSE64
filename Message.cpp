#include <SKSE/PapyrusNativeFunctions.h>

#include "Message.h"
#include "Relocation.h"
#include "ConfigStore.h"
#include "InterfaceManager.h"
#include "IOManager.h"
#include "Utility.h"

namespace Message
{
	float prevTime = 0.0f;
	float prevTime2 = 0.0f;
	float prevTime3 = 0.0f;

	class MinimapUpdateSink : public BSTEventSink<PlayerUpdateEvent>
	{
	public:
		virtual	EventResult	ReceiveEvent(PlayerUpdateEvent* evn, void* dispatcher)
		{
			ConfigStore::ConfigData* data = ConfigStore::GetInstance();
			// minimap update
			if (prevTime == 0.0f || prevTime + (1.0f / data->targetFPS) < *Relocation::fSystemTimeClock_Ptr)
			{
				InterfaceManager::UpdateMinimap();
				prevTime = *Relocation::fSystemTimeClock_Ptr;
			}
			// localmap update
			if (prevTime2 == 0.0f || prevTime2 + data->localUpdateDelay < *Relocation::fSystemTimeClock_Ptr)
			{
				Utility::doUpdate = true;
				prevTime2 = *Relocation::fSystemTimeClock_Ptr;
			}
			// play sound
			if (data->playSoundEffect && (prevTime3 == 0.0f || prevTime3 + data->soundDelay < *Relocation::fSystemTimeClock_Ptr))
			{
				if (data->modActive && !InterfaceManager::GetHided() && !data->soundFile.empty())
				{
					//Utility::PlayOneShotWrapper();
					Relocation::PlayOneShot(data->soundFile.c_str(), 0);
				}
				prevTime3 = *Relocation::fSystemTimeClock_Ptr;
			}

			return kEvent_Continue;
		};
	};

	MinimapUpdateSink minimapUpdateSink;

	class OnPauseMenuCloseEvent : public BSTEventSink<MenuOpenCloseEvent>
	{
	public:
		virtual	EventResult	ReceiveEvent(MenuOpenCloseEvent* evn, void* dispatcher)
		{
			if (_stricmp(evn->menuName.c_str(), "PauseMenu") == 0 && !evn->isOpen)
			{
				// delay 100ms for prevent crash
				prevTime += 0.1f;
				ConfigStore::GetMCMConfig();
			}
			else if (_stricmp(evn->menuName.c_str(), "FaderMenu") == 0 && !evn->isOpen) // Loading menu off?
			{
				prevTime += 0.1f;
				ConfigStore::GetMCMConfig();
			}
			if (evn->menuName == "HUDMenu")
			{
				if (evn->isOpen)
				{
					InterfaceManager::OpenMenu();
				}
				else
				{
					InterfaceManager::CloseMenu();
				}
			}
			if ((*g_ui) != nullptr && (*g_ui)->IsMenuOpen("HUDMenu") && !(*g_ui)->IsMenuOpen("MiniMap"))
			{
				InterfaceManager::OpenMenu();
			}
			InterfaceManager::CheckHideUI(evn->menuName.c_str(), evn->isOpen);
			return kEvent_Continue;
		};
	};

	OnPauseMenuCloseEvent pauseMenuCloseSink;

	void SKSEMessageHandler(SKSEMessagingInterface::Message* msg)
	{
		switch (msg->type)
		{
		case SKSEMessagingInterface::kMessage_GameLoaded:
		{
			Relocation::BSTGlobalEvent_RegisterSink_PlayerUpdateEvent(&minimapUpdateSink);
			(*g_ui)->menuOpenCloseEventSource.AddEventSink(&pauseMenuCloseSink);
			ConfigStore::Clear();
			IOManager::SetStatic();
			InterfaceManager::RegisterMenu();
			_DMESSAGE("Game Loaded");
		}
		break;
		}
	}
};