#pragma once

#ifndef hRelocation
#define hRelocation

#include <SKSE/GameData.h>
#include <SKSE/GameExtraData.h>
#include <SKSE/GameEvents.h>
#include <SKSE/PapyrusArgs.h>
#include <SKSE/GameMenus.h>
#include <SKSE/GameReferences.h>

#include "DataStruct.h"
#include <SKSE/ScaleformLoader.h>

DECLARE_EVENT_DISPATCHER(TESEquipEvent, 0x00442870);

namespace Relocation
{
	extern RelocPtr <void> UnderarmorScale_BranchStart;
	extern RelocPtr <void> AddToBiped_Dest;
	extern RelocPtr <void> AddToBiped_Start;
	extern RelocPtr <void> AddToBiped_Start2; // 201
	extern RelocPtr <void> AddToBiped_Start3; // 120
	extern RelocPtr <void> AddToBiped_Start4; // 128
	extern RelocPtr <void> EquipObject_Start;
	extern RelocPtr <void> AttachModsForBipedObject_Start;
	extern RelocPtr <void> AttachModsForBipedObject_Start2;
	extern RelocPtr <void> HideShowHair_Dest;
	extern RelocPtr <void> HideShowMeatcap_Dest;

	extern RelocPtr<bool> bBipedAnimUpdateSkins;
	extern RelocPtr<const char*> aSlanguageGener;
	extern RelocPtr<UInt32> hNull;
	extern RelocPtr<void*> ViewCasterBase;
	extern RelocPtr<void*> ActorEquipManager;
	extern RelocPtr<BGSKeyword*> PowerArmorArmorKeyword;
	extern RelocPtr<BGSKeyword*> PowerArmorKeyword;
	extern RelocPtr<BGSKeyword*> FeaturedKeyword;
	extern RelocPtr <float> fSystemTimeClock_Ptr;

	extern RelocPtr <BSScaleformExternalTexture*> LocalMapTexture;
	extern RelocPtr <LocalMapRenderer*> qLocalMapRenderer;

	extern RelocPtr<CombatManager*> QCombatManager;;

	typedef void(*_PlayOneShot)(char const*, UInt32);
	extern RelocAddr <_PlayOneShot> PlayOneShot;

	typedef void(*_BSTGlobalEvent_RegisterSink_PlayerUpdateEvent)(BSTEventSink<struct PlayerUpdateEvent>*);
	extern RelocAddr <_BSTGlobalEvent_RegisterSink_PlayerUpdateEvent> BSTGlobalEvent_RegisterSink_PlayerUpdateEvent;

	typedef bool(*_GetStaticFunction)(VirtualMachine*, char const*, char const*, VMType_Ptr&, VMFunc_Ptr&);
	extern RelocAddr <_GetStaticFunction> GetStaticFunction;

	typedef float(*_GetPlayerHeading)(PlayerCharacter*);
	extern RelocAddr <_GetPlayerHeading> GetPlayerHeading;

	typedef UInt32(*_GetKeywordItemCount)(TESObjectREFR*, BGSKeyword*, UInt32, bool equipedOnly, void*);
	extern RelocAddr <_GetKeywordItemCount> GetKeywordItemCount_Original;

	typedef float(*_GetStealthPoints)(CombatManager*, Actor*);
	extern RelocAddr <_GetStealthPoints> GetStealthPoints;

	typedef bool(*_CalculateLOS)(Actor*, TESObjectREFR*, NiPoint3*, NiPoint3*, SInt32);
	extern RelocAddr <_CalculateLOS> CalculateLOS;

	typedef void(*_GetLocalMapTexturePath)(TESObjectCELL*, BSString&);
	extern RelocAddr <_GetLocalMapTexturePath> GetExteriorLocalMapTexturePath;
	extern RelocAddr <_GetLocalMapTexturePath> GetInteriorLocalMapTexturePath;

	typedef NiTexture* (*_NiTexture_Create)(const BSFixedString*, bool, bool);
	extern RelocAddr <_NiTexture_Create> NiTexture_Create;

	typedef bool (*_IsLocked)(GameVM*, UInt32, TESObjectREFR*);
	extern RelocAddr <_IsLocked> IsLocked;

	typedef bool(*_IsCrimeToActivate)(TESObjectREFR*);
	extern RelocAddr <_IsCrimeToActivate> IsCrimeToActivate;

	typedef bool(*_IsFollowing)(Actor*, Actor*);
	extern RelocAddr <_IsFollowing> IsFollowing;

	typedef bool(*_AddTexture)(BSScaleformImageLoader*, void*);
	extern RelocAddr <_AddTexture> AddTexture;

	typedef const char* (*_GetTextureName)(void*);
	extern RelocAddr <_GetTextureName> GetTextureName;

	typedef void (*_RenderLocalMap)();
	extern RelocAddr <_RenderLocalMap> RenderLocalMap;
	extern RelocAddr <_RenderLocalMap> CreateRenderer;
	extern RelocAddr <_RenderLocalMap> DestroyRenderer;



	typedef void* (*_operator_new)(UInt64);
	extern RelocAddr <_operator_new> operator_new;

	typedef void (*_operator_delete)(void*);
	extern RelocAddr <_operator_delete> operator_delete;


	typedef void(*_EnumReferencesCloseToRef)(DataHandler*, TESObjectREFR*, float, NiPoint3*, float, bool(__fastcall*)(TESObjectREFR*, void*), void*);
	extern RelocAddr <_EnumReferencesCloseToRef> EnumReferencesCloseToRef_Original;

	typedef bool (*_IsDead)(Actor*, bool);
	extern RelocAddr <_IsDead> IsDead_Original;


	typedef BGSInventoryList* (*_BGSInventoryList_ctor)(BGSInventoryList*, TESContainer*, UInt32*);
	extern RelocAddr <_BGSInventoryList_ctor> BGSInventoryList_ctor; // 0x80

	typedef void (*_AddItem)(BGSInventoryList*, TESBoundObject*, UInt32, ExtraDataList*, UInt16);
	extern RelocAddr <_AddItem> AddItem;

	typedef void (*_AddIventoryItem)(BGSInventoryList*, BGSInventoryItem&);
	extern RelocAddr <_AddIventoryItem> AddInventoryItem;

	typedef void (*_RemoveIventoryItem)(BGSInventoryList*, UInt32);
	extern RelocAddr <_RemoveIventoryItem> RemoveInventoryItem;



	typedef TBO_InstanceData* (*_GetInstanceData)(BGSInventoryItem*, UInt32);
	extern RelocAddr <_GetInstanceData> GetInstanceData;



	typedef void (*_SendEquip)(TESObjectREFR**, UInt32, UInt16, UInt32, bool);
	extern RelocAddr <_SendEquip> SendEquip;



	typedef ExtraDataList* (*_ExtraDataList_ctor)(ExtraDataList*);
	extern RelocAddr <_ExtraDataList_ctor> ExtraDataList_ctor; // 0x28

	typedef BSExtraData* (*_AddExtra)(ExtraDataList*, BSExtraData*);
	extern RelocAddr <_AddExtra> AddExtra;

	typedef BSExtraData* (*_GetExtraData)(ExtraDataList*, UInt8);
	extern RelocAddr <_GetExtraData> GetExtraData;

	typedef void (*_InitItem)(ExtraDataList*, TESForm*);
	extern RelocAddr <_InitItem> InitItem;

	typedef void (*_SetOverrideName)(ExtraDataList*, const char*);
	extern RelocAddr <_SetOverrideName> SetOverrideName;

	typedef TBO_InstanceData* (*_CreateInstanceData)(ExtraDataList*, TESBoundObject*, bool setDisplayName);
	extern RelocAddr <_CreateInstanceData> CreateInstanceData;

	typedef void (*_GetUniqueID)(ExtraDataList*, UInt16*, UInt32*);
	extern RelocAddr <_GetUniqueID> GetUniqueID;



	typedef UInt32* (*_PlaceAtMeFunctionBase)(UInt32*, TESObjectREFR*, TESBoundObject*, SInt32, SInt32, SInt32, float, bool);
	extern RelocAddr <_PlaceAtMeFunctionBase> PlaceAtMeFunctionBase;

	typedef void (*_CreateInventoryList)(TESObjectREFR*, TESContainer*);
	extern RelocAddr <_CreateInventoryList> CreateInventoryList;

	typedef void (*_OpenContainerMenu)(TESObjectREFR*, SInt32, bool);
	extern RelocAddr <_OpenContainerMenu> OpenContainerMenu;

	typedef void (*_SetWantsDelete)(TESObjectREFR*, bool);
	extern RelocAddr <_SetWantsDelete> SetWantsDelete;

	typedef void(*_SetOwner)(TESObjectREFR*, TESForm*);
	extern RelocAddr <_SetOwner> SetOwner;

	typedef void(*_NiPointer_TESObjectREFR_Release)(UInt64, TESObjectREFR*);
	extern RelocAddr <_NiPointer_TESObjectREFR_Release> NiPointer_TESObjectREFR_Release;



	typedef BSFixedString* (*_GetDisplayName)(ExtraTextDisplayData*, TESBoundObject*);
	extern RelocAddr <_GetDisplayName> GetDisplayName;

	typedef bool (*_HasCustomName)(ExtraTextDisplayData*);
	extern RelocAddr <_HasCustomName> HasCustomName;



	typedef bool (*_HasKeywordDirect)(TESObjectREFR*, BGSKeyword*);
	extern RelocAddr <_HasKeywordDirect> HasKeywordDirect;



	typedef BGSObjectInstanceExtra* (*_BGSObjectInstanceExtra_ctor)(BGSObjectInstanceExtra*);
	extern RelocAddr <_BGSObjectInstanceExtra_ctor> BGSObjectInstanceExtra_ctor; // 0x28

	typedef void (*_AddMod)(BGSObjectInstanceExtra*, BGSMod::Attachment::Mod*, UInt8, UInt8, bool);
	extern RelocAddr <_AddMod> AddMod;

	typedef void (*_CreateBaseInstanceData)(BGSObjectInstanceExtra*, TESBoundObject*, TBO_InstanceData**);
	extern RelocAddr <_CreateBaseInstanceData> CreateBaseInstanceData;

	typedef UInt32(*_LoadAndAttach3D)(BGSObjectInstanceExtra*, NiNode*, void*, TESObjectREFR*, void*, bool, TBO_InstanceData*, bool);
	extern RelocAddr <_LoadAndAttach3D> LoadAndAttach3D_Original;

	typedef BGSObjectInstanceExtra* (*_MakeCopy)(BGSObjectInstanceExtra*);
	extern RelocAddr <_MakeCopy> MakeCopy;



	typedef BGSObjectInstanceExtra* (*_GetEquippedObjectInstanceExtra)(Actor*, TESObjectARMO*);
	extern RelocAddr <_GetEquippedObjectInstanceExtra> GetEquippedObjectInstanceExtra_Original;

	typedef ExtraDataList* (*_GetEquippedObjectExtra)(Actor*, TESObjectARMO*);
	extern RelocAddr <_GetEquippedObjectExtra> GetEquippedObjectExtra_Modded;

	typedef bool(*_ActorInPowerArmor)(Actor*);
	extern RelocAddr <_ActorInPowerArmor> ActorInPowerArmor;

	typedef UInt32* (*_GetActorHandle)(Actor*, UInt32*);
	extern RelocAddr <_GetActorHandle> GetActorHandle;



	typedef bool(*_EquipObject)(void*, Actor*, BGSObjectInstance*, UInt32, UInt32, BGSEquipSlot*, bool, bool, bool, bool, bool);
	extern RelocAddr <_EquipObject> EquipObject_Original;

	typedef bool(*_UnequipObject)(void*, Actor*, BGSObjectInstance*, UInt32, BGSEquipSlot*, UInt32, bool, bool, bool, bool, BGSEquipSlot*);
	extern RelocAddr <_UnequipObject> UnequipObject_Original;



	typedef bool (*_AddToBiped)(BGSObjectInstance*, TESRace*, ActorEquipData**, UInt32, UInt16);
	extern RelocAddr <_AddToBiped> AddToBiped_Original;

	typedef bool (*_LookupActorByHandle)(UInt32*, Actor**);
	extern RelocAddr <_LookupActorByHandle> LookupActorByHandle;

	typedef TESObjectREFR* (*_FindReferenceFor3D)(NiAVObject*);
	extern RelocAddr <_FindReferenceFor3D> FindReferenceFor3D;

	typedef BGSEquipSlot* (*_GetEquipSlotForForm)(TESForm*, TBO_InstanceData*);
	extern RelocAddr <_GetEquipSlotForForm> GetEquipSlotForForm;

	typedef BGSObjectInstanceExtra* (*_AttachModsForBipedObject)(UInt32 bipedObjectEnum, Actor* actor, NiNode* node, void* argType, bool, BGSObjectInstance*, void*);
	extern RelocAddr <_AttachModsForBipedObject> AttachModsForBipedObject_Original;

	typedef void (*_CallGlobalFunctionNoWait)(VirtualMachine*, UInt32, void*, const BSFixedString*, const BSFixedString*, VMValue*);
	extern RelocAddr <_CallGlobalFunctionNoWait> CallGlobalFunctionNoWait;

	typedef void(*_InitModHavok)(bhkWorld*, NiAVObject*, BGSObjectInstanceExtra*, bool);
	extern RelocAddr <_InitModHavok> InitModHavok_Original;

	typedef void(*_PostAttach3D)(TBO_InstanceData*, NiNode*, TESObjectREFR*);
	extern RelocAddr <_PostAttach3D> PostAttach3D_Original;

	typedef bhkWorld* (*_GetbhkWorld)(TESObjectCELL*);
	extern RelocAddr <_GetbhkWorld> GetbhkWorld_Original;

	typedef bool(*_UseArmor)(TESObjectARMO*, TESRace*);
	extern RelocAddr <_UseArmor> UseArmor;

	typedef UInt32* (*_QActivatePickRef)(void*, UInt32*);
	extern RelocAddr <_QActivatePickRef> QActivatePickRef;

	typedef bool (*_CreateInstanceDataForObjectAndExtra)(TESBoundObject*, ExtraDataList*, UInt32*, bool);
	extern RelocAddr <_CreateInstanceDataForObjectAndExtra> CreateInstanceDataForObjectAndExtra;

	typedef bool (*_HideShowMeatcap_Original)(NiAVObject*, UInt32, UInt32, bool, bool);
	extern RelocAddr <_HideShowMeatcap_Original> HideShowMeatcap_Original;

	typedef bool (*_DoAdvanceMovie)(IMenu*, float);
	extern RelocAddr <_DoAdvanceMovie> DoAdvanceMovie;

	extern _GetEquippedObjectExtra GetEquippedObjectExtra_Original;

	typedef float (*_MCM_GetModSettingFloat)(StaticFunctionTag* base, BSFixedString asModName, BSFixedString asModSetting);
	extern _MCM_GetModSettingFloat MCM_GetModSettingFloat;

	typedef BSFixedString(*_MCM_GetModSettingString)(StaticFunctionTag* base, BSFixedString asModName, BSFixedString asModSetting);
	extern _MCM_GetModSettingString MCM_GetModSettingString;

	typedef void(*_MCM_SetModSettingFloat)(StaticFunctionTag* base, BSFixedString asModName, BSFixedString asModSetting, float abValue);
	extern _MCM_SetModSettingFloat MCM_SetModSettingFloat;
};

#endif