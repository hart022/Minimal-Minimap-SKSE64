#include <xbyak/xbyak.h>

#include <SKSE_common/BranchTrampoline.h>
#include <SKSE_common/SafeWrite.h>

#include <SKSE/GameRTTI.h>
#include <SKSE/GameExtraData.h>
#include <SKSE/GameReferences.h>
#include <SKSE/NiNodes.h>

#include "Trampoline.h"
#include "Relocation.h"
#include "DataStruct.h"
#include "ConfigStore.h"
#include "Utility.h"

namespace Trampoline
{
	typedef void(*_DrawWorld_Render_UI_Dest)();
	RelocPtr<_DrawWorld_Render_UI_Dest> DrawWorld_Render_UI_Dest(0x00D3877D);

	void DrawWorld_Render_UI_Hook(BSGraphics_Renderer* renderer)
	{
		renderer->ResetState();
		Utility::RegisterLocalMapImage();
		renderer->ResetState();
	}

	bool DoAlloc()
	{
		/*
		if (!g_localTrampoline.Create(10))
		{
			_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
			return false;
		}

		/**/

		if (!g_branchTrampoline.Create(14))
		{
			_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
			return false;
		}
		/**/

		{
			//org = (_Interface3DRenderFn)*Interface3DRenderFn;
			//SafeWrite64(Interface3DRenderFn.GetUIntPtr(), (UInt64)Interface3DRenderFn_Hook);
			g_branchTrampoline.Write5Call(DrawWorld_Render_UI_Dest.GetUIntPtr(), (uintptr_t)DrawWorld_Render_UI_Hook);

			//UInt8 buf[] = { 0x90, 0x90 , 0x90 , 0x90 , 0x90 };
			//SafeWriteBuf(RelocationManager::s_baseAddr + 0x1DD6CF, buf, sizeof(buf));
		}
		return true;
	}
};
