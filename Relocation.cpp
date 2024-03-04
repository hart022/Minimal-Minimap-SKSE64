#include "Relocation.h"

namespace Relocation
{
	RelocPtr <void> UnderarmorScale_BranchStart(0x005C63EE);
	RelocPtr <void> AddToBiped_Dest(0x001BE6D0 + 0xB6);
	RelocPtr <void> AddToBiped_Start(0x001BE6D0 + 0x171);
	RelocPtr <void> AddToBiped_Start2(0x005B93A1); // 201
	RelocPtr <void> AddToBiped_Start3(0x004288D0); // 120
	RelocPtr <void> AddToBiped_Start4(0x00421B38); // 128
	RelocPtr <void> EquipObject_Start(0x00E1CB59);
	RelocPtr <void> AttachModsForBipedObject_Start(0x001BF8AD);
	RelocPtr <void> AttachModsForBipedObject_Start2(0x001C214B);
	RelocPtr <void> HideShowHair_Dest(0x005AE640 + 0x17);
	RelocPtr <void> HideShowMeatcap_Dest(0x003FEE66);

	RelocPtr<bool> bBipedAnimUpdateSkins(0x058D4B00);
	RelocPtr<const char*> aSlanguageGener(0x036D4B40);
	RelocPtr<UInt32> hNull(0x038CCE04);
	RelocPtr<void*> ViewCasterBase(0x058E2B30);
	RelocPtr<void*> ActorEquipManager(0x059D75C8);
	RelocPtr<BGSKeyword*> PowerArmorArmorKeyword(0x058DE990);
	RelocPtr<BGSKeyword*> PowerArmorKeyword(0x058DE920);
	RelocPtr<BGSKeyword*> FeaturedKeyword(0x059D7430);

	RelocPtr <BSScaleformExternalTexture*> LocalMapTexture(0x05A67EC0);
	RelocPtr <LocalMapRenderer*> qLocalMapRenderer(0x05A67EB0);

	RelocPtr<CombatManager*> QCombatManager(0x059D9DC8);

	RelocPtr <float> fSystemTimeClock_Ptr(0x05AC8E84);

	RelocAddr <_CalculateLOS> CalculateLOS(0x010B1800);

	RelocAddr <_IsCrimeToActivate> IsCrimeToActivate(0x00410140);

	RelocAddr <_IsFollowing> IsFollowing(0x00DA6F10);

	RelocAddr <_AddTexture> AddTexture(0x022E5AD0);

	RelocAddr <_GetTextureName> GetTextureName(0x02114620);

	RelocAddr <_RenderLocalMap> RenderLocalMap(0x00B88F50);
	RelocAddr <_RenderLocalMap> CreateRenderer(0x00B88E30);
	RelocAddr <_RenderLocalMap> DestroyRenderer(0x00B88EB0);

	RelocAddr <_IsLocked> IsLocked(0x01408E70);

	RelocAddr <_PlayOneShot> PlayOneShot(0x0082F610);

	RelocAddr <_GetKeywordItemCount> GetKeywordItemCount_Original(0x003FFEA0);

	RelocAddr <_GetPlayerHeading> GetPlayerHeading(0x00EA6AF0);

	RelocAddr <_GetStealthPoints> GetStealthPoints(0x01085040);

	RelocAddr <_PlaceAtMeFunctionBase> PlaceAtMeFunctionBase(0x005121D0);

	RelocAddr <_GetStaticFunction> GetStaticFunction(0x0273D730);

	RelocAddr <_CreateInventoryList> CreateInventoryList(0x0040A370);

	RelocAddr <_OpenContainerMenu> OpenContainerMenu(0x01264F40);

	RelocAddr <_SetWantsDelete> SetWantsDelete(0x004115C0);

	RelocAddr <_SetOwner> SetOwner(0x0047F0E0);

	RelocAddr <_NiPointer_TESObjectREFR_Release> NiPointer_TESObjectREFR_Release(0x0000B0E0);

	RelocAddr <_IsDead> IsDead_Original(0x00D89D40);

	RelocAddr <_EnumReferencesCloseToRef> EnumReferencesCloseToRef_Original(0x00112670);

	RelocAddr <_AddToBiped> AddToBiped_Original(0x00332AC0);

	RelocAddr <_FindReferenceFor3D> FindReferenceFor3D(0x00408580);

	RelocAddr <_GetEquipSlotForForm> GetEquipSlotForForm(0x0013F630);

	RelocAddr <_HasKeywordDirect> HasKeywordDirect(0x001A2120);

	RelocAddr <_SendEquip> SendEquip(0x00435CB0);

	RelocAddr <_HasCustomName> HasCustomName(0x000DAE80);

	RelocAddr <_EquipObject> EquipObject_Original(0x00E1BCD0);

	RelocAddr <_UnequipObject> UnequipObject_Original(0x00E1C0B0);

	RelocAddr <_ActorInPowerArmor> ActorInPowerArmor(0x009CC980);

	RelocAddr <_CallGlobalFunctionNoWait> CallGlobalFunctionNoWait(0x01451A80);

	RelocAddr <_CreateBaseInstanceData> CreateBaseInstanceData(0x0003DBA0);

	RelocAddr <_AttachModsForBipedObject> AttachModsForBipedObject_Original(0x001C4CD0);

	RelocAddr <_LoadAndAttach3D> LoadAndAttach3D_Original(0x0003BC90);

	RelocAddr <_InitModHavok> InitModHavok_Original(0x00402D50);

	RelocAddr <_PostAttach3D> PostAttach3D_Original(0x00324280);

	RelocAddr <_GetbhkWorld> GetbhkWorld_Original(0x003B49A0);

	RelocAddr <_operator_new> operator_new(0x0030D0);

	RelocAddr <_operator_delete> operator_delete(0x003110);

	RelocAddr <_BGSInventoryList_ctor> BGSInventoryList_ctor(0x001AB0E0); // 0x80

	RelocAddr <_AddItem> AddItem(0x001AB460);

	RelocAddr <_ExtraDataList_ctor> ExtraDataList_ctor(0x00080750); // 0x28

	RelocAddr <_AddExtra> AddExtra(0x00042FA0);

	RelocAddr <_BGSObjectInstanceExtra_ctor> BGSObjectInstanceExtra_ctor(0x00042480); // 0x28

	RelocAddr <_AddMod> AddMod(0x0003BF70);

	RelocAddr <_InitItem> InitItem(0x00084700);

	RelocAddr <_GetExtraData> GetExtraData(0x000436A0);

	RelocAddr <_GetUniqueID> GetUniqueID(0x00097C20);

	RelocAddr <_GetEquippedObjectInstanceExtra> GetEquippedObjectInstanceExtra_Original(0x009D2BF0);

	RelocAddr <_CreateInstanceData> CreateInstanceData(0x0008A260);

	RelocAddr <_GetDisplayName> GetDisplayName(0x000C0A00);

	RelocAddr <_GetInstanceData> GetInstanceData(0x001A61F0);

	RelocAddr <_SetOverrideName> SetOverrideName(0x00089440);

	RelocAddr <_GetEquippedObjectExtra> GetEquippedObjectExtra_Modded(0x009D2B50);

	RelocAddr <_UseArmor> UseArmor(0x00332DB0);

	RelocAddr <_QActivatePickRef> QActivatePickRef(0x009DDDF0);

	RelocAddr <_AddIventoryItem> AddInventoryItem(0x001ABA00);

	RelocAddr <_RemoveIventoryItem> RemoveInventoryItem(0x001AD570);

	RelocAddr <_MakeCopy> MakeCopy(0x0003C970);

	RelocAddr <_LookupActorByHandle> LookupActorByHandle(0x00023740);

	RelocAddr <_GetActorHandle> GetActorHandle(0x00D89690);

	RelocAddr <_CreateInstanceDataForObjectAndExtra> CreateInstanceDataForObjectAndExtra(0x000517B0);

	RelocAddr <_HideShowMeatcap_Original> HideShowMeatcap_Original(0x0055D120);

	RelocAddr <_DoAdvanceMovie> DoAdvanceMovie(0x0210EF10);

	RelocAddr <_GetLocalMapTexturePath> GetExteriorLocalMapTexturePath(0x003BC600);
	RelocAddr <_GetLocalMapTexturePath> GetInteriorLocalMapTexturePath(0x003BC6C0);

	RelocAddr <_NiTexture_Create> NiTexture_Create(0x01BA5220);

	_GetEquippedObjectExtra GetEquippedObjectExtra_Original;

	RelocAddr <_BSTGlobalEvent_RegisterSink_PlayerUpdateEvent> BSTGlobalEvent_RegisterSink_PlayerUpdateEvent(0x00C2AC40);

	_MCM_GetModSettingFloat MCM_GetModSettingFloat;

	_MCM_SetModSettingFloat MCM_SetModSettingFloat;

	_MCM_GetModSettingString MCM_GetModSettingString;
};