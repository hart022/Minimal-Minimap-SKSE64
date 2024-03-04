#include <sstream>

#include <SKSE/GameRTTI.h>
#include <SKSE/GameMenus.h>
#include <SKSE/ScaleformLoader.h>
#include <SKSE/GameThreads.h>

#include "Relocation.h"
#include "ConfigStore.h"
#include "InterfaceManager.h"
#include "IOManager.h"
#include "Utility.h"
#include <thread>

#define is_interior(cell) ((cell) != nullptr && ((cell)->flags & 1) != 0)

// whole code from https://github.com/WirelessLan/SKSEPlugins/blob/CoordiSaver-v0.5/CoordiSaver/ScaleformManager.cpp
namespace InterfaceManager
{
	bool hotkeyRegistered = false;

	std::string interfaceName = "MiniMap";

	std::set<std::string, InsensitiveStringLess> HideUISet;

	class OutfitInterfaceMenu : public GameMenuBase
	{
	public:
		OutfitInterfaceMenu()
		{
			this->depth = 5;
			flags = IMenu::kFlag_DoNotDeleteOnClose | IMenu::kFlag_DisableInteractive | IMenu::kFlag_DoNotPreventGameSave;
			bool result = CALL_MEMBER_FN((*g_scaleformManager), LoadMovie)(this, movie, interfaceName.c_str(), "root1", 4);
		}

		virtual void* Unk_05()
		{
			return __super::Unk_05();
		}
	};

	static OutfitInterfaceMenu* interfaceTarget = nullptr;

	void CheckHideUI(std::string name, bool isOpen)
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		if (interfaceTarget != nullptr && data->modActive)
		{
			bool needHide = false;

			for (UInt32 i = 0; i < (*g_ui)->menuStack.count; i++)
			{
				IMenu* m = (*g_ui)->menuStack.entries[i];
				if (m != nullptr && HideUISet.count(m->menuName.c_str()) > 0)
				{
					needHide = true;
				}
			}

			GFxMovieRoot* movieRoot = interfaceTarget->movie->movieRoot;
			GFxValue root, visible;

			if (!movieRoot->GetVariable(&root, "root")) {
				_ERROR("Couldn't get a root");
				return;
			}

			visible.SetBool(!needHide);

			root.Invoke("UpdateVisible", nullptr, &visible, 1);
		}
	}

	bool GetHided()
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		if (interfaceTarget != nullptr && data->modActive)
		{
			bool needHide = false;

			for (UInt32 i = 0; i < (*g_ui)->menuStack.count; i++)
			{
				IMenu* m = (*g_ui)->menuStack.entries[i];
				if (m != nullptr && HideUISet.count(m->menuName.c_str()) > 0)
				{
					needHide = true;
				}
			}

			return needHide;
		}
		return true;
	}

	void OpenMenu()
	{
		CALL_MEMBER_FN((*g_uiMessageManager), SendUIMessage)(BSFixedString(interfaceName.c_str()), MessageType::kMessage_Open);
	}

	void CloseMenu()
	{
		CALL_MEMBER_FN((*g_uiMessageManager), SendUIMessage)(BSFixedString(interfaceName.c_str()), MessageType::kMessage_Close);
	}

	IMenu* CreateMenu()
	{
		interfaceTarget = new OutfitInterfaceMenu();
		return interfaceTarget;
	}

	void RegisterMenu()
	{
		if (*g_ui != nullptr && !(*g_ui)->IsMenuRegistered(BSFixedString(interfaceName.c_str())))
		{
			(*g_ui)->Register(interfaceName.c_str(), CreateMenu);
		}

		_DMESSAGE("Menu Registered");
	}

	void InsertMenuName(std::vector<std::string>* names)
	{
		for (std::string name : *names)
		{
			HideUISet.insert(name);
		}
	}

	void SetInterfaceName(std::string name)
	{
		interfaceName = name;
	}

	class ScaleformLogHandler : public GFxFunctionHandler
	{
	public:
		virtual void Invoke(Args* args) {
			if (args->numArgs >= 1)
			{
				_MESSAGE(args->args[0].GetString());
			}
		}
	};

	class GetRenderRariusHandler : public GFxFunctionHandler
	{
	public:
		virtual void Invoke(Args* args) {
			ConfigStore::ConfigData* data = ConfigStore::GetInstance();
			args->result->SetNumber(static_cast<double>(data->renderRadius));
		}
	};

	bool RegisterScaleform(GFxMovieView* view, GFxValue* SKSE_root) {
		RegisterFunction<ScaleformLogHandler>(SKSE_root, view->movieRoot, "Log");
		RegisterFunction<GetRenderRariusHandler>(SKSE_root, view->movieRoot, "Radius");

		return true;
	}

	void UpdateMinimap()
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		if (interfaceTarget != nullptr && data->modActive)
		{
			GFxMovieRoot* movieRoot = interfaceTarget->movie->movieRoot;
			GFxValue root;

			if (!movieRoot->GetVariable(&root, "root")) {
				_ERROR("Couldn't get a root");
				return;
			}

			float nowRadius = Utility::GetNowRadius();

			GFxValue args[3];

			float posx = (Utility::playerX - (*g_player)->pos.x) * data->renderRadius / nowRadius;
			float posy = (Utility::playerY - (*g_player)->pos.y) * data->renderRadius / nowRadius;

			float cellRotation = 0.0f;
			if ((*g_player)->parentCell != nullptr && ((*g_player)->parentCell->flags & 1) != 0 && (*g_player)->parentCell->extraDataList != nullptr)
			{
				ExtraNorthRotation* extra = (ExtraNorthRotation*)(*g_player)->parentCell->extraDataList->GetByType(ExtraDataType::kExtraData_NorthRotation);
				if (extra != nullptr)
				{
					cellRotation = extra->data;
				}
			}

			if (cellRotation != 0.0f)
			{
				double tempX = sin(cellRotation) * posy + cos(cellRotation) * posx;
				double tempY = cos(cellRotation) * posy - sin(cellRotation) * posx;
				posx = tempX;
				posy = tempY;
			}

			args[0].SetNumber(static_cast<double>(Utility::mapResult / (nowRadius * data->localMapMult)));
			args[1].SetNumber(static_cast<double>(posx));
			args[2].SetNumber(static_cast<double>(posy));

			/**/
			movieRoot->Invoke("root.UpdateImage", nullptr, args, 3);
			/**/

			GFxValue arr = DataStruct::BuildRefrArray(movieRoot);
			movieRoot->Invoke("root.UpdateMinimap", nullptr, &arr, 1);
		}
	}

	void UpdateConfig()
	{
		if (interfaceTarget != nullptr)
		{
			ConfigStore::ConfigData* data = ConfigStore::GetInstance();
			GFxMovieRoot* movieRoot = interfaceTarget->movie->movieRoot;
			GFxValue root, confArray, confValue;

			if (!movieRoot->GetVariable(&root, "root")) {
				_ERROR("Couldn't get a root");
				return;
			}

			movieRoot->CreateArray(&confArray);

			for (double value : data->interfaceConfigs)
			{
				confValue.SetNumber(value);
				confArray.PushBack(&confValue);
			}

			root.Invoke("UpdateConfig", nullptr, &confArray, 1);

			root.Invoke("UpdateVisible", nullptr, &GFxValue(data->modActive), 1);
		}
	}
};

#undef is_interior