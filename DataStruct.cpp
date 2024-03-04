#include <SKSE/GameCamera.h>
#include <SKSE/NiNodes.h>
#include <SKSE/PapyrusVM.h>

#include "DataStruct.h"
#include "ConfigStore.h"
#include "Relocation.h"
#include "Utility.h"

STATIC_ASSERT(offsetof(PlayerCharacter, unkC90) == 0xD08);
STATIC_ASSERT(offsetof(PlayerCharacter, unkB68) == 0xB80);

#define TypePair(x) { DisplayType::x, #x }

#define is_third_person_alt (((*g_player)->unkC90[(0xDF8 - 0xD08) / sizeof(UInt64)] & 0x010000000000000ull) != 0)

#define is_interior(cell) ((cell) != nullptr && ((cell)->flags & 1) != 0)

std::atomic<bool> is_collect = false;

namespace DataStruct
{
	std::map<UInt32, std::string> DisplayTypeMap = {
		TypePair(Ally), TypePair(Enemy), TypePair(DeadBody), TypePair(Container), TypePair(Portal),
		TypePair(Terminal), TypePair(Featured), TypePair(Locked), TypePair(Crime), TypePair(Follower), TypePair(Quest)
	};

	GFxValue BuildRefrArray(GFxMovieRoot* movieRoot)
	{
		ConfigStore::ConfigData* data = ConfigStore::GetInstance();

		double playerRotation = static_cast<double>((*g_player)->rot.z);
		double cameraRotation = static_cast<double>((*g_playerCamera)->unk194);
		double cameraFoV = static_cast<double>((*g_playerCamera)->fDefault1stPersonFOV);
		if (is_third_person_alt)
		{
			cameraFoV = static_cast<double>((*g_playerCamera)->fDefaultWorldFov);
		}

		double cellRotation = 0.0f;
		if (is_interior((*g_player)->parentCell) && (*g_player)->parentCell->extraDataList != nullptr)
		{
			ExtraNorthRotation* extra = (ExtraNorthRotation*)(*g_player)->parentCell->extraDataList->GetByType(ExtraDataType::kExtraData_NorthRotation);
			if (extra != nullptr)
			{
				cellRotation = static_cast<double>(extra->data);
			}
		}

		bool isInCombat = Relocation::GetStealthPoints(*Relocation::QCombatManager, *g_player) <= data->targetStealthPoint;

		GFxValue result;

		if (data->modActive && movieRoot != nullptr && !is_collect)
		{
			is_collect = true;

			float nowRadius = Utility::GetNowRadius();

			std::vector<TESObjectREFR*> refList;
			Relocation::EnumReferencesCloseToRef_Original(*g_dataHandler, *g_player, nowRadius, &(*g_player)->pos, nowRadius, [](TESObjectREFR* refr, void* acc) -> bool {
				if (acc != nullptr && refr != nullptr && refr != *g_player && !(refr->flags & (TESForm::kFlag_IsDisabled | TESForm::kFlag_IsDeleted)))
				{
					std::vector<TESObjectREFR*>* accList = (std::vector<TESObjectREFR*>*)acc;
					accList->push_back(refr);
				}
				return false;
			}, &refList);

			movieRoot->CreateArray(&result);

			result.PushBack(&GFxValue(180.0 * (playerRotation + cellRotation) / MATH_PI));
			result.PushBack(&GFxValue(data->rotateMinimap));

			result.PushBack(&GFxValue(180.0 * (cameraRotation + cellRotation) / MATH_PI));
			result.PushBack(&GFxValue(data->useCamRotation));

			result.PushBack(&GFxValue(MATH_PI * cameraFoV / 180.0));

			if (data->playerAffectZoom)
			{
				result.PushBack(&GFxValue(static_cast<double>(nowRadius / data->radius)));
			}
			else
			{
				result.PushBack(&GFxValue(1.0));
			}

			ExtraFollower* extra = (ExtraFollower*)(*g_player)->extraDataList->GetByType(ExtraDataType::kExtraData_Follower);

			std::set<UInt32> handles;

			if (data->displaySymbol[DisplayType::Quest])
			{
				for (UInt32 i = 0; i < (*g_player)->objData.count; i++)
				{
					BGSQuestObjective* obj = (BGSQuestObjective*)(*g_player)->objData.entries[i].data;
					if (obj != nullptr && obj->objectiveState == 0x01 && obj->count > 0 && obj->owner != nullptr && (obj->owner->flags & (1 << 0x0B)))
					{
						for (UInt32 j = 0; j < obj->count; j++)
						{
							TESQuestTarget* target = obj->arr[j];
							if (target != nullptr && target->data.count > 0)
							{
								for (UInt32 k = 0; k < target->data.count; k++)
								{
									TESQuestTarget::REF_DATA* refData = target->data.entries[k];
									if (refData != nullptr)
									{
										UInt32 handle = refData->handle;
										if (refData->unkarr18.count > 0)
										{
											TESObjectREFR* refr = refData->unkarr18.entries[0];
											if (refr != nullptr)
											{
												handle = refr->CreateRefHandle();
											}
										}

										if (handle != 0)
										{
											handles.insert(handle);
										}
									}
								}
							}
						}
					}
				}
			}

			if (data->customMarkerAsObj)
			{
				UInt32 customMarkerHandle = ((*g_player)->unkB68[(0xBC0 - 0xB80) / sizeof(UInt64)] >> 0x20);
				TeleportPath* path = (TeleportPath*)(*g_player)->unkB68[(0xBC8 - 0xB80) / sizeof(UInt64)];

				if (path != nullptr && path->unkarr18.count > 0)
				{
					TESObjectREFR* refr = path->unkarr18.entries[0];
					if (refr != nullptr)
					{
						customMarkerHandle = refr->CreateRefHandle();
					}
				}

				if (customMarkerHandle != 0)
				{
					handles.insert(customMarkerHandle);
				}
			}

			if (data->paMarkerAsObj)
			{
				UInt32 paMarkerHandle = (UInt32)(*g_player)->unkC90[(0xD90 - 0xD08) / sizeof(UInt64)];
				TeleportPath* path = (TeleportPath*)(*g_player)->unkC90[(0xD98 - 0xD08) / sizeof(UInt64)];

				if (path != nullptr && path->unkarr18.count > 0)
				{
					TESObjectREFR* refr = path->unkarr18.entries[0];
					if (refr != nullptr)
					{
						paMarkerHandle = refr->CreateRefHandle();
					}
				}

				if (paMarkerHandle != 0)
				{
					handles.insert(paMarkerHandle);
				}
			}

			for (TESObjectREFR* refr : refList)
			{
				if (refr != nullptr && refr->parentCell != nullptr && (*g_player)->parentCell != nullptr)
				{
					if (is_interior((*g_player)->parentCell) && refr->parentCell != (*g_player)->parentCell)
					{
						continue;
					}

					if (!data->checkLOS || Relocation::CalculateLOS(*g_player, refr, &(*g_player)->pos, &refr->pos, -1))
					{
						DisplayType type = DisplayType::None;
						switch (refr->formType)
						{
						case FormType::kFormType_ACHR:
						{
							Actor* tActor = (Actor*)refr;
							if (Relocation::IsFollowing(tActor, *g_player) && extra != nullptr && extra->data.GetItemIndex(ExtraFollower::FollowerInfo(refr->CreateRefHandle())) != -1)
							{
								if (data->displaySymbol[DisplayType::Follower])
								{
									type = DisplayType::Follower;
								}
							}
							else if (Relocation::IsDead_Original(tActor, true))
							{
								if (data->displaySymbol[DisplayType::DeadBody])
								{
									type = DisplayType::DeadBody;
								}

								if (Relocation::HasKeywordDirect(refr, *Relocation::FeaturedKeyword) || Relocation::GetKeywordItemCount_Original(refr, *Relocation::FeaturedKeyword, 0, false, nullptr) > 0)
								{
									if (data->displaySymbol[DisplayType::Featured])
									{
										type = DisplayType::Featured;
									}
								}
							}
							else
							{
								if (Utility::CalcRealHostileToActor(*g_player, tActor) || Utility::CalcRealHostileToActor(tActor, *g_player))
								{
									if (data->displaySymbol[DisplayType::Enemy])
									{
										type = DisplayType::Enemy;
									}
								}
								else
								{
									if (data->displaySymbol[DisplayType::Ally])
									{
										type = DisplayType::Ally;
									}
								}
							}
						}
						break;
						default:
						{
							if (data->displaySymbol[DisplayType::Locked] && Relocation::IsLocked(*g_gameVM, 0, refr))
							{
								type = DisplayType::Locked;
							}
							else if (data->displaySymbol[DisplayType::Crime] && Relocation::IsCrimeToActivate(refr))
							{
								type = DisplayType::Crime;
							}
							else if (refr->extraDataList != nullptr && refr->extraDataList->HasType(ExtraDataType::kExtraData_Teleport))
							{
								if (data->displaySymbol[DisplayType::Portal])
								{
									type = DisplayType::Portal;
								}
							}
							else if (Relocation::HasKeywordDirect(refr, *Relocation::FeaturedKeyword) || Relocation::GetKeywordItemCount_Original(refr, *Relocation::FeaturedKeyword, 0, false, nullptr) > 0)
							{
								if (data->displaySymbol[DisplayType::Featured])
								{
									type = DisplayType::Featured;
								}
							}
							else if (refr->inventoryList != nullptr)
							{
								if (data->displaySymbol[DisplayType::Container])
								{
									type = DisplayType::Container;
								}
							}
							else if (refr->baseForm != nullptr && refr->baseForm->formType == FormType::kFormType_TERM)
							{
								if (data->displaySymbol[DisplayType::Terminal])
								{
									type = DisplayType::Terminal;
								}
							}
						}
						break;
						}

						UInt32 handle = refr->CreateRefHandle();
						if (handles.count(handle) > 0 && data->displaySymbol[DisplayType::Quest])
						{
							type = DisplayType::Quest;
							handles.erase(handle);
						}

						if (type != DisplayType::None && (!isInCombat || !data->skipAtCombatSymbol[type]))
						{
							GFxValue value;
							movieRoot->CreateObject(&value);

							double posX = (refr->pos.x - (*g_player)->pos.x) / nowRadius;
							double posY = (refr->pos.y - (*g_player)->pos.y) / nowRadius;
							double posZ = refr->pos.z - (*g_player)->pos.z;

							if (cellRotation != 0.0)
							{
								double tempX = sin(cellRotation) * posY + cos(cellRotation) * posX;
								double tempY = cos(cellRotation) * posY - sin(cellRotation) * posX;
								posX = tempX;
								posY = tempY;
							}

							value.SetMember("offsetx", &GFxValue(posX));
							value.SetMember("offsety", &GFxValue(-posY));
							value.SetMember("offsetz", &GFxValue(posZ));
							value.SetMember("over", &GFxValue(type == DisplayType::Quest && data->drawObjectiveOver));
							value.SetMember("type", &GFxValue(type));

							result.PushBack(&value);
						}
					}
				}
			}

			if (data->drawObjectiveOver)
			{
				TESObjectREFR* refr = nullptr;
				for (UInt32 handle : handles)
				{
					LookupREFRByHandle(&handle, &refr);

					if (refr != nullptr)
					{
						GFxValue value;
						movieRoot->CreateObject(&value);

						double posX = (refr->pos.x - (*g_player)->pos.x);
						double posY = (refr->pos.y - (*g_player)->pos.y);
						double posZ = refr->pos.z - (*g_player)->pos.z;

						double distance = sqrt(posX * posX + posY * posY);

						if (distance < nowRadius)
						{
							posX /= nowRadius;
							posY /= nowRadius;
						}
						else
						{
							posX = posX * 1.05 / distance;
							posY = posY * 1.05 / distance;
						}

						if (data->ignoreHeightForOver)
						{
							posZ = 0.0;
						}

						if (cellRotation != 0.0)
						{
							double tempX = sin(cellRotation) * posY + cos(cellRotation) * posX;
							double tempY = cos(cellRotation) * posY - sin(cellRotation) * posX;
							posX = tempX;
							posY = tempY;
						}

						value.SetMember("offsetx", &GFxValue(posX));
						value.SetMember("offsety", &GFxValue(-posY));
						value.SetMember("offsetz", &GFxValue(posZ));
						value.SetMember("over", &GFxValue(true));
						value.SetMember("type", &GFxValue(DisplayType::Quest));

						result.PushBack(&value);
					}
				}

				handles.clear();

				UInt32 tInt = 0;
				LookupREFRByHandle(&tInt, &refr);
			}

			is_collect = false;
		}
		return result;
	}
};

#undef is_third_person_alt

#undef is_interior

#undef TypePair