#pragma once

#ifndef hDataStruct
#define hDataStruct

#include <vector>
#include <SKSE/GameObjects.h>
#include <SKSE/GameCamera.h>
#include <SKSE/GameExtraData.h>
#include <SKSE/ScaleformValue.h>
#include <SKSE/PapyrusNativeFunctions.h>
#include <SKSE/BSLight.h>

class TeleportPath
{
public:
	tArray<UInt64> unkarr00;
	tArray<TESObjectREFR*> unkarr18;
	UInt32 unk30;
	UInt32 unk34;
	UInt32 unk38;
	UInt32 unk3C;
	UInt32 unk40;
	UInt32 unk44;
	UInt8 unk48;
};
STATIC_ASSERT(sizeof(TeleportPath) == 0x50);

class TESQuestTarget
{
public:
	class REF_DATA : public TeleportPath
	{
	public:
		UInt32 handle; // 50
		UInt32 pad54;
		UInt64 unk58;
		UInt8 unk60;
	};

	UInt8 unk00;
	UInt8 unk01;
	UInt16 unk02;
	UInt32 unk04;
	void* condition; // 08, tescondition
	UInt64 unk10;
	UInt64 unk18;
	tArray<REF_DATA*> data;
};
STATIC_ASSERT(sizeof(TESQuestTarget::REF_DATA) == 0x68);

enum ObjectiveStateEnum : UInt8
{
	// 2 - 3 == completed
	// 4 - 5 == failed
	// 1, 3, 5 == displayed
};

class BGSQuestObjective
{
public:
	BSFixedString str; // 0x00
	TESQuest* owner; // 0x08
	TESQuestTarget** arr; // 0x1C
	UInt32 count; // 0x18
	UInt16 index; // 0x1C
	UInt8 unk1E;
	UInt8 objectiveState; // 0x1F
	UInt32 flag;

	bool GetDisplayed()
	{
		UInt8 state = this->objectiveState;
		if (state == 1 || state == 3 || state == 5)
		{
			return true;
		}
		return false;
	}
};

STATIC_ASSERT(sizeof(BGSQuestObjective) == 0x28);

struct CombatManager
{

};

struct TESEquipEvent
{
	TESObjectREFR* refr;
	UInt32 armorID;
	UInt32 refrID;
	UInt16 uniqueID;
	bool isEquipped;
};
STATIC_ASSERT(offsetof(TESEquipEvent, isEquipped) == 0x12);

struct BSScaleformExternalTexture
{
	NiTexture* texture;
	UInt32 renderTarget;
	UInt32 pad0C;
	BSFixedString name;
};

class NiCamera : public NiAVObject
{
public:
	UInt64 unk120[(0x160 - 0x120) / sizeof(UInt64)];
	NiFrustum frustum;
};

class LocalMapCamera : public TESCamera
{
public:
	UInt64 unk38[(0x58 - 0x38) / sizeof(UInt64)];
	NiCamera* unk58;
	UInt64 unk60;

	void SetMinFrustum(float a, float b)
	{
		return (RelocAddr<void(*)(LocalMapCamera*, float, float)>(0x001DCDA0))(this, a, b);
	}

	void SetExtents(NiPoint3* a, NiPoint3* b)
	{
		return (RelocAddr<void(*)(LocalMapCamera*, NiPoint3*, NiPoint3*)>(0x001DCD60))(this, a, b);
	}

	void Reset()
	{
		return (RelocAddr<void(*)(LocalMapCamera*)>(0x001DCB10))(this);
	}

	void SetInitialPosition(NiPoint3* a)
	{
		return (RelocAddr<void(*)(LocalMapCamera*, NiPoint3*)>(0x001DCB30))(this, a);
	}

	void SetZRotation(float a)
	{
		return (RelocAddr<void(*)(LocalMapCamera*, float)>(0x001DCBA0))(this, a);
	}
};

//2B0
struct LocalMapRenderer
{
	UInt64 unk00[(0x1A0 - 0x00) / sizeof(UInt64)];
	LocalMapCamera camera;
	UInt64 last;
	UInt64 unk210[(0x2A0 - 0x210) / sizeof(UInt64)];
	UInt32 unk2A0;
	UInt32 unk2A4;
	UInt64 unk2A8;

	static LocalMapRenderer* ctor(bool a)
	{
		LocalMapRenderer* result = (LocalMapRenderer*)Heap_Allocate(sizeof(LocalMapRenderer));
		(RelocAddr<void(*)(LocalMapRenderer*, bool)>(0x001DCDE0))(result, a);
		return result;
	}

	void dtor()
	{
		(RelocAddr<void(*)(LocalMapRenderer*)>(0x001DCF80))(this);
		Heap_Free(this);
	}

	void SetupFogOfWar()
	{
		return (RelocAddr<void(*)(LocalMapRenderer*)>(0x001DDC10))(this);
	}

	NiTexture* Render(bool a)
	{
		return (RelocAddr<NiTexture* (*)(LocalMapRenderer*, bool)>(0x001DD040))(this, a);
	}
};
STATIC_ASSERT(offsetof(LocalMapRenderer, last) == 0x208);
STATIC_ASSERT(sizeof(LocalMapRenderer) == 0x2B0);

struct RenderTargetProperties
{
	UInt32 width;
	UInt32 height;
	UInt32 unk08;
	UInt32 unk0C;
	UInt32 unk10;
	UInt32 unk14;
	UInt32 unk18;
	UInt32 unk1C;
};

struct BSGraphics_RenderTargetManager
{
	static BSGraphics_RenderTargetManager* Get()
	{
		return (RelocPtr<BSGraphics_RenderTargetManager>(0x0384FD30));
	}

	BSRenderData* SaveRenderTargetToTexture(SInt32 a, bool b, bool c, UInt32 d)
	{
		return (RelocAddr<BSRenderData* (*)(BSGraphics_RenderTargetManager*, SInt32, bool, bool, UInt32)>(0x01D32910))(this, a, b, c, d);
	}

	void SetCurrentViewportForceToRenderTargetDimensions()
	{
		return (RelocAddr<void (*)(BSGraphics_RenderTargetManager*)>(0x01D31B20))(this);
	}

	void AcquireRenderTarget(SInt32 a)
	{
		return (RelocAddr<void (*)(BSGraphics_RenderTargetManager*, SInt32)>(0x01D329D0))(this, a);
	}

	void SetCurrentRenderTarget(SInt32 a, SInt32 b, UInt32 c)
	{
		return (RelocAddr<void (*)(BSGraphics_RenderTargetManager*, SInt32, SInt32, UInt32)>(0x01D318B0))(this, a, b, c);
	}

	void SetCurrentDepthStencilTarget(SInt32 a, UInt32 b, SInt32 c, bool d)
	{
		return (RelocAddr<void (*)(BSGraphics_RenderTargetManager*, SInt32, UInt32, SInt32, bool)>(0x01D31940))(this, a, b, c, d);
	}

	void CreateRenderTarget(SInt32 a, RenderTargetProperties* b, bool c)
	{
		return (RelocAddr<void (*)(BSGraphics_RenderTargetManager*, SInt32, RenderTargetProperties*, bool)>(0x01D30E90))(this, a, b, c);
	}

	void ReleaseRenderTarget(SInt32 a)
	{
		return (RelocAddr<void (*)(BSGraphics_RenderTargetManager*, SInt32)>(0x01D32A40))(this, a);
	}
};

struct BSGraphics_Renderer
{
	void Begin(UInt32 a)
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*, UInt32)>(0x01D0B420))(this, a);
	}

	void End()
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*)>(0x01D0B670))(this);
	}

	void ResetState()
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*)>(0x01D11DA0))(this);
	}

	void Lock()
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*)>(0x01D0B390))(this);
	}

	bool TryLock()
	{
		return (RelocAddr<bool(*)(BSGraphics_Renderer*)>(0x01D0B3D0))(this);
	}

	void Unlock()
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*)>(0x01D0B3B0))(this);
	}

	void SetContext(void* a)
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*, void*)>(0x01D14CB0))(this, a);
	}

	void Flush()
	{
		return (RelocAddr<void(*)(BSGraphics_Renderer*)>(0x01D0B760))(this);
	}

	NiTexture* CreateTextureFromBuffer(UInt32 a, UInt32 b, void* c, bool d, bool e, UInt32 f, UInt32 g, bool h)
	{
		NiTexture* result = (RelocAddr<NiTexture* (*)(BSFixedString&, bool, bool)>(0x01BA5220))(BSFixedString(), false, false);
		if (result != nullptr)
		{
			BSRenderData* data = (RelocAddr<BSRenderData* (*)(BSGraphics_Renderer*, UInt32, UInt32, void*, bool, bool, UInt32, UInt32, bool)>(0x01D0EE30))(this, a, b, c, d, e, f, g, h);
			result->rendererData = data;
		}
		return result;
	}

	static BSGraphics_Renderer* Get()
	{
		return (RelocPtr<BSGraphics_Renderer>(0x061E0900));
	}
};

struct PlayerUpdateEvent
{

};

struct BGSObjectInstance
{
	TESForm* form;
	TBO_InstanceData* instance;
};

struct InsensitiveStringLess
{
	bool operator() (const std::string& lhs, const std::string& rhs) const
	{
		return _stricmp(lhs.c_str(), rhs.c_str()) < 0;
	}
};

enum DisplayType : UInt32
{
	None = 0,
	Ally,
	Enemy,
	DeadBody,
	Container,
	Portal,
	Terminal,
	Featured,
	Locked,
	Crime,
	Follower,
	Quest,
	_MAX
};

class ExtraFollower : public BSExtraData
{
public:
	struct FollowerInfo
	{
		FollowerInfo(UInt32 handle)
		{
			this->handle = handle;
		}

		UInt32 handle;
		UInt32 handle2; // ???

		bool operator==(FollowerInfo& rhs)
		{
			return this->handle == rhs.handle;
		}

		bool operator==(FollowerInfo* rhs)
		{
			return this->handle == rhs->handle;
		}
	};

	tArray<FollowerInfo> data;
};

class ExtraNorthRotation : public BSExtraData
{
public:
	float data;
};

class TESPackage; // form

struct BGSRefAliasInstanceData
{
	TESQuest* quest;
	BGSRefAlias* alias;
	tArray<TESPackage*>* array;
};

class ExtraReferenceHandle : public BSExtraData
{
public:
	UInt32 originalHandle;
	UInt32 containerHandle;
};

class ExtraAliasInstanceArray : public BSExtraData
{
public:
	tArray<BGSRefAliasInstanceData> arr;
};

class VMType_Ptr
{
public:
	VMObjectTypeInfo* m_pObject;	// 00

	inline VMType_Ptr(VMObjectTypeInfo* pObject = nullptr)
	{
		m_pObject = pObject;
		if (m_pObject) m_pObject->AddRef();
	}

	inline ~VMType_Ptr()
	{
		if (m_pObject) m_pObject->Release();
	}

	inline operator bool() const
	{
		return m_pObject != nullptr;
	}

	inline operator VMObjectTypeInfo* () const
	{
		return m_pObject;
	}

	inline VMObjectTypeInfo& operator*() const
	{
		return *m_pObject;
	}

	inline VMObjectTypeInfo* operator->() const
	{
		return m_pObject;
	}

	inline VMType_Ptr& operator=(const VMType_Ptr& rhs)
	{
		if (m_pObject != rhs.m_pObject)
		{
			if (rhs) rhs.m_pObject->AddRef();
			if (m_pObject) m_pObject->Release();

			m_pObject = rhs.m_pObject;
		}

		return *this;
	}

	inline VMType_Ptr& operator=(VMObjectTypeInfo* rhs)
	{
		if (m_pObject != rhs)
		{
			if (rhs) rhs->AddRef();
			if (m_pObject) m_pObject->Release();

			m_pObject = rhs;
		}

		return *this;
	}

	inline bool operator==(VMObjectTypeInfo* pObject) const
	{
		return m_pObject == pObject;
	}

	inline bool operator!=(VMObjectTypeInfo* pObject) const
	{
		return m_pObject != pObject;
	}

	inline bool operator==(const VMType_Ptr& ptr) const
	{
		return m_pObject == ptr.m_pObject;
	}

	inline bool operator!=(const VMType_Ptr& ptr) const
	{
		return m_pObject != ptr.m_pObject;
	}
};

class VMFunc_Ptr
{
public:
	NativeFunction* m_pObject;	// 00

	inline VMFunc_Ptr(NativeFunction* pObject = nullptr)
	{
		m_pObject = pObject;
		if (m_pObject)
		{
			InterlockedIncrement(&m_pObject->refCount);
		}
	}

	inline ~VMFunc_Ptr()
	{
		if (m_pObject)
		{
			if (InterlockedDecrement(&m_pObject->refCount) == 0)
			{
				delete m_pObject;
			}
		}
	}

	inline operator bool() const
	{
		return m_pObject != nullptr;
	}

	inline operator NativeFunction* () const
	{
		return m_pObject;
	}

	inline NativeFunction& operator*() const
	{
		return *m_pObject;
	}

	inline NativeFunction* operator->() const
	{
		return m_pObject;
	}

	inline VMFunc_Ptr& operator=(const VMFunc_Ptr& rhs)
	{
		if (m_pObject != rhs.m_pObject)
		{
			if (rhs)
			{
				InterlockedIncrement(&rhs->refCount);
			}
			if (m_pObject)
			{
				if (InterlockedDecrement(&m_pObject->refCount) == 0)
				{
					delete m_pObject;
				}
			}

			m_pObject = rhs.m_pObject;
		}

		return *this;
	}

	inline VMFunc_Ptr& operator=(NativeFunction* rhs)
	{
		if (m_pObject != rhs)
		{
			if (rhs)
			{
				InterlockedIncrement(&rhs->refCount);
			}
			if (m_pObject)
			{
				if (InterlockedDecrement(&m_pObject->refCount) == 0)
				{
					delete m_pObject;
				}
			}

			m_pObject = rhs;
		}

		return *this;
	}

	inline bool operator==(NativeFunction* pObject) const
	{
		return m_pObject == pObject;
	}

	inline bool operator!=(NativeFunction* pObject) const
	{
		return m_pObject != pObject;
	}

	inline bool operator==(const VMFunc_Ptr& ptr) const
	{
		return m_pObject == ptr.m_pObject;
	}

	inline bool operator!=(const VMFunc_Ptr& ptr) const
	{
		return m_pObject != ptr.m_pObject;
	}
};

namespace DataStruct
{
	extern std::map<UInt32, std::string> DisplayTypeMap;

	GFxValue BuildRefrArray(GFxMovieRoot* movieRoot);
};

#endif

