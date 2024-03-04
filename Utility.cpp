#include <sstream>
#include <fstream>
#include <regex>

#include <SKSE/GameRTTI.h>
#include <SKSE/GameObjects.h>
#include <SKSE/GameData.h>

#include "Utility.h"
#include "Relocation.h"
#include <MinimalMinimap/ConfigStore.h>

#define is_interior(cell) ((cell) != nullptr && ((cell)->flags & 1) != 0)

namespace Utility
{
	std::string Base64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	SKSETaskInterface* g_task = nullptr;

	std::atomic<float> mapResult = -1.0f;
	std::atomic<float> playerX = -1.0f;
	std::atomic<float> playerY = -1.0f;
	std::atomic<bool> doUpdate = false;
	std::atomic<bool> renderNow = false;
	
	std::vector<std::string>* GetLinesFromFile(std::string dataPath)
	{
		std::vector<std::string>* result = new std::vector<std::string>();
		std::ifstream ifs(dataPath);
		if (!ifs.is_open())
		{
			return result;
		}
		std::string tString = "", efshName;
		UInt32 formID = 0;
		TESEffectShader* tShader = nullptr;
		while (getline(ifs, tString))
		{
			tString = std::regex_replace(tString, std::regex("#.*$"), "");
			result->push_back(tString);
		}
		return result;
	}

	// from https://ssungkang.tistory.com/entry/C-string-%EB%AC%B8%EC%9E%90%EC%97%B4-%EB%82%98%EB%88%84%EB%8A%94-split
	std::vector<std::string> split_string(std::string inputString, char delimiter) {
		std::vector<std::string> answer;
		std::stringstream ss(inputString);
		std::string temp;
		while (getline(ss, temp, delimiter)) {
			temp = std::regex_replace(temp, std::regex("^\\s+"), ""); // strip whitespace
			temp = std::regex_replace(temp, std::regex("\\s+$"), ""); // strip whitespace
			answer.push_back(temp);
		}
		return answer;
	}

	std::string Base64Decode(std::string value)
	{
		std::string str = std::regex_replace(value, std::regex("[^0-9a-zA-Z+/]"), "");
		if (str.empty())
		{
			return str;
		}
		str += "AAAA";
		UInt32 strLength = str.length();
		UInt32 tChars = 0;
		std::vector<char> arr;
		for (UInt32 i = 0; i < strLength - 4; i += 4)
		{
			tChars = Base64Table.find(str[i + 3ul]) + (Base64Table.find(str[i + 2ul]) << 6) + (Base64Table.find(str[i + 1ul]) << 0x0C) + (Base64Table.find(str[i]) << 0x12);
			arr.push_back((tChars >> 0x10) & 0xFF);
			arr.push_back((tChars >> 0x08) & 0xFF);
			arr.push_back(tChars & 0xFF);
		}
		arr.push_back(0);
		return arr.data();
	}

	std::string Base64Encode(std::string str)
	{
		if (str.empty())
		{
			return str;
		}
		std::vector<unsigned char> arr;
		UInt32 strLength = str.length();
		for (UInt32 i = 0; i < strLength; i++)
		{
			arr.push_back((unsigned char)str[i]);
		}
		std::vector<char> arr2;
		for (UInt32 i = 0; i < strLength - 2; i += 3)
		{
			arr2.push_back(Base64Table[arr[i] >> 0x02]);
			arr2.push_back(Base64Table[((arr[i] & 0x03) << 0x04) + (arr[i + 1] >> 0x04)]);
			arr2.push_back(Base64Table[((arr[i + 1] & 0x0F) << 0x02) + (arr[i + 2] >> 0x06)]);
			arr2.push_back(Base64Table[arr[i + 2] & 0x3F]);
		}
		if (strLength % 3 == 1)
		{
			arr2.push_back(Base64Table[arr[strLength - 1] >> 0x02]);
			arr2.push_back(Base64Table[(arr[strLength - 1] & 0x03) << 0x04]);
			arr2.push_back('=');
			arr2.push_back('=');
		}
		else if (strLength % 3 == 2)
		{
			arr2.push_back(Base64Table[arr[strLength - 2] >> 0x02]);
			arr2.push_back(Base64Table[((arr[strLength - 2] & 0x03) << 0x04) + (arr[strLength - 1] >> 0x04)]);
			arr2.push_back(Base64Table[(arr[strLength - 1] & 0x0F) << 0x02]);
			arr2.push_back('=');
		}
		arr2.push_back(0);
		return arr2.data();
	}

	UInt32 GetEquippedMask(Actor* actor)
	{
		UInt32 result = 0;
		if (actor != nullptr && actor->inventoryList != nullptr)
		{
			BGSInventoryList* tList = actor->inventoryList;
			for (UInt32 i = 0; i < tList->items.count; i++)
			{
				BGSInventoryItem tItem = tList->items.entries[i];
				BGSInventoryItem::Stack* tStack = tItem.stack;
				while (tStack != nullptr)
				{
					if (tStack->flags & BGSInventoryItem::Stack::kFlagEquipped)
					{
						TESObjectARMO* tArmor = DYNAMIC_CAST(tItem.form, TESForm, TESObjectARMO);
						if (tArmor != nullptr)
						{
							UInt32 mask = tArmor->bipedObject.GetSlotMask();
							result |= mask;
						}
						break;
					}
					tStack = tStack->next;
				}
			}
		}
		return result;
	}

	bool HasCustomName(ExtraTextDisplayData* display)
	{
		return display != nullptr && (display->message != nullptr || display->quest != nullptr || display->type == -2);
	}

	std::string GetStringByID(UInt32 target, char delimeter)
	{
		if (target != 0)
		{
			std::stringstream stream;
			ModInfo* info;
			if ((target >> 24) == 0xFF)
			{
				return "";
			}
			else if ((target >> 24) == 0xFE)
			{
				(*g_dataHandler)->modList.lightMods.GetNthItem((target >> 12) & 0xFFF, info);
				if (info != nullptr)
				{
					stream << info->name << delimeter << std::hex << (target & 0xFFF);
				}
			}
			else
			{
				(*g_dataHandler)->modList.loadedMods.GetNthItem(target >> 24, info);
				if (info != nullptr)
				{
					stream << info->name << delimeter << std::hex << (target & 0xFFFFFF);
				}
			}

			return stream.str();
		}
		return "";
	}

	std::string GetStringByForm(TESForm* tForm, char delimeter)
	{
		if (tForm != nullptr && LookupFormByID(tForm->formID) == tForm)
		{
			return GetStringByID(tForm->formID, delimeter);
		}
		return "";
	}

	UInt32 GetIDByString(std::string str, char delimeter)
	{
		try
		{
			if (str.find(delimeter) != -1)
			{
				std::vector<std::string> result = Utility::split_string(str, delimeter);
				const ModInfo* mod = (*g_dataHandler)->LookupModByName(result.at(0).c_str());
				if (mod != NULL)
				{
					return mod->GetFormID(std::stoi(result.at(1), nullptr, 16));
				}
			}
		}
		catch (...)
		{

		}
		return 0;
	}

	TESForm* GetFormByString(std::string str, char delimeter)
	{
		UInt32 id = GetIDByString(str, delimeter);
		if (id != 0)
		{
			return LookupFormByID(id);
		}
		return nullptr;
	}

	RelocAddr <bool (*)(Actor*, Actor*)> IsInCombatWithActor(0x00DFF960);
	RelocAddr <UInt32(*)(Actor*, Actor*)> GetFactionFightReaction(0x00DA2FB0);
	RelocAddr <bool (*)(Actor*, Actor*, UInt8)> GetShouldAttackActor(0x00D906E0);
	RelocPtr <ActorValueInfo*> QAggression(0x058CFFF0 + 0x10);

	bool CalcRealHostileToActor(Actor* from, Actor* to)
	{
		if (IsInCombatWithActor(from, to))
		{
			return true;
		}
		if (GetShouldAttackActor(from, to, 0xFF))
		{
			return true;
		}
		TESNPC* npc = DYNAMIC_CAST(to->baseForm, TESForm, TESNPC);
		if (npc != nullptr)
		{
			if ((npc->aiForm.unk08[0] >> 0x14) & 1)
			{
				if (to->actorValueOwner.GetValue(*QAggression) != -1.0f)
				{
					if (((npc->aiForm.unk08[0] >> 0x20) & 0xFFFF) || (npc->aiForm.unk08[1] & 0xFFFF))
					{
						if (GetFactionFightReaction(from, to) <= 1)
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	// from https://github.com/maximusmaxy/ScreenArcherMenu/blob/97ce9c286b2ffcd91eed9d51f847631ff51664d6/SAM/papyrus.cpp#L49
	void* GetFunctionProxy(const char* scope, const char* name)
	{
		void* result = nullptr;
		VMType_Ptr info;
		VMFunc_Ptr func;
		if (Relocation::GetStaticFunction((*g_gameVM)->m_virtualMachine, scope, name, info, func))
		{
			result = (void*)*(((UInt64*)(NativeFunction*)func) + (0x50 / 0x08));
		}
		return result;
	}

	void CalcExtents_Pending(LocalMapRenderer* renderer, NiPoint3& lt, NiPoint3& rt, NiPoint3& lb)
	{
		NiCamera* ca = renderer->camera.unk58;

		if (ca != nullptr)
		{
			float fL = ca->frustum.m_fLeft;
			float fR = ca->frustum.m_fRight;
			float fT = ca->frustum.m_fTop;
			float fB = ca->frustum.m_fBottom;

			float pX = ca->m_worldTransform.pos.x;
			float pY = ca->m_worldTransform.pos.y;
			float pZ = ca->m_worldTransform.pos.z;

			float tYX = ca->m_worldTransform.rot.arr[0x10 / sizeof(float)];
			float tYY = ca->m_worldTransform.rot.arr[0x14 / sizeof(float)];
			float tYZ = ca->m_worldTransform.rot.arr[0x18 / sizeof(float)];

			float tZX = ca->m_worldTransform.rot.arr[0x20 / sizeof(float)];
			float tZY = ca->m_worldTransform.rot.arr[0x24 / sizeof(float)];
			float tZZ = ca->m_worldTransform.rot.arr[0x28 / sizeof(float)];

			lt.x = tZX * fL + pX + tYX * fT;
			lt.y = tZY * fL + pY + tYY * fT;
			lt.z = tZZ * fL + pZ + tYZ * fT;

			rt.x = tZX * fR + pX + tYX * fT;
			rt.y = tZY * fR + pY + tYY * fT;
			rt.z = tZZ * fR + pZ + tYZ * fT;

			lb.x = tZX * fL + pX + tYX * fB;
			lb.y = tZX * fL + pY + tYY * fB;
			lb.z = tZX * fL + pZ + tYZ * fB;
		}
	}

	NiTexture* prevTexture = nullptr;

	void RegisterLocalMapImage()
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();

		if (data->modActive && data->useLocalMap && doUpdate && !renderNow)
		{
			renderNow = true;
			doUpdate = false;

			NiTexture* texture = nullptr;
			float a = 0.0f;

			LocalMapRenderer* renderer = LocalMapRenderer::ctor(false);

			a = *(RelocPtr<float>(0x03781378));
			renderer->camera.SetMinFrustum(a, a * 0.5625f);

			NiPoint3 b, c, am = { 1, 1, 1 };
			am *= data->localMapRect;
			b = c = (*g_player)->pos;
			b -= am;
			c += am;
			//(RelocAddr<void(*)(NiPoint3&, NiPoint3&)>(0x00B88BF0))(b, c);

			renderer->camera.SetExtents(&c, &b);

			renderer->SetupFogOfWar();

			renderer->camera.Reset();

			renderer->camera.SetInitialPosition(&(*g_player)->pos);

			renderer->camera.SetExtents(&c, &b);

			if ((*g_player)->parentCell != nullptr)
			{
				float cellRotation = 0.0f;
				if ((*g_player)->parentCell != nullptr && ((*g_player)->parentCell->flags & 1) != 0 && (*g_player)->parentCell->extraDataList != nullptr)
				{
					ExtraNorthRotation* extra = (ExtraNorthRotation*)(*g_player)->parentCell->extraDataList->GetByType(ExtraDataType::kExtraData_NorthRotation);
					if (extra != nullptr)
					{
						cellRotation = extra->data;
					}
				}
				renderer->camera.SetZRotation(-cellRotation);
			}

			RelocPtr<bool> bFogOfWarEnabled(0x036DA580);

			bool value = *bFogOfWarEnabled;
			*bFogOfWarEnabled = false;

			texture = renderer->Render(true);

			*bFogOfWarEnabled = value;

			NiPoint3 d, e, f;
			CalcExtents_Pending(renderer, d, e, f);
			NiPoint3 g = d - e;

			mapResult = sqrtf(g.x * g.x + g.y * g.y);

			/**/
			BSScaleformImageLoader* imageLoader = (*g_scaleformManager)->imageLoader;
			if (imageLoader != nullptr)
			{
				if (prevTexture != nullptr)
				{
					imageLoader->UnmountImage(&prevTexture);
					prevTexture->DecRef();
				}
				texture->name = "MinimapImage";
				texture->IncRef();
				prevTexture = texture;
				imageLoader->MountImage(&texture);
				if (*g_player != nullptr)
				{
					playerX = (*g_player)->pos.x;
					playerY = (*g_player)->pos.y;
				}
			}
			else
			{
				texture->DecRef();
			}
			/**/

			renderer->dtor();

			renderNow = false;
		}
		else if (!data->modActive || !data->useLocalMap)
		{
			mapResult = -1.0f;
		}
	}

	void PlayOneShotWrapper()
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();

		if (data->playSoundEffect)
		{
			struct ID
			{
				UInt64 unk00;
				UInt32 unk08;
			};

			RelocPtr<void*>AudioManager(0x5B0D208);
			RelocAddr<char const *(*)(char*, UInt32, const char*, const char*)>PrepareName(0x01B7CD40);
			RelocAddr<void(*)(ID*, char*)>ID_ctor(0x01B6F0E0);
			RelocAddr<void(*)(void*, UInt64*, ID*, UInt32, UInt8, const char*)>GetSoundHandleByFile(0x01ACA3D0);
			RelocAddr<void(*)(UInt64*)>AudioManager_Play(0x01AC8040);
			RelocAddr<void(*)(UInt64*, float)>AudioManager_SetVolume(0x01AC8450);
			RelocAddr<void(*)(UInt64*, NiPoint3*)>AudioManager_SetPosition(0x01AC8680);

			UInt64 handle = 0;
			char buf[512];
			ID id;

			PrepareName(buf, MAX_PATH, data->soundFile.c_str(), "Sound\\");
			ID_ctor(&id, buf);
			GetSoundHandleByFile(*AudioManager, &handle, &id, 0, 0x80u, data->soundFile.c_str());
			//AudioManager_SetPosition(&handle, &(*g_player)->pos);
			AudioManager_Play(&handle);
		}
	}

	float GetNowRadius()
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();
		float nowRadius = data->radius;

		if (is_interior((*g_player)->parentCell))
		{
			nowRadius *= data->interiorMapMult;
		}

		if (data->nowZoomLevel != 0.0f)
		{
			nowRadius *= data->zoomMultList[static_cast<SInt32>(data->nowZoomLevel) - 1];
		}
		return nowRadius;
	}
};

#undef is_interior
