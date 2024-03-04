#pragma once

#ifndef hOutfitSystemInterface
#define hOutfitSystemInterface

#include <SKSE/ScaleformMovie.h>
#include <SKSE/ScaleformValue.h>

namespace InterfaceManager
{
	void OpenMenu();
	void CloseMenu();
	void RegisterMenu();

	bool RegisterScaleform(GFxMovieView* view, GFxValue* SKSE_root);
	void UpdateConfig();
	void UpdateMinimap();

	void CheckHideUI(std::string name, bool isOpen);
	void InsertMenuName(std::vector<std::string>* names);
	void SetInterfaceName(std::string name);

	bool GetHided();
}

#endif

