#include "Serialize.h"
#include "ConfigStore.h"
#include "Global.h"

namespace Serialize
{
	UInt32 RecordSignature = 'MMAP';
	UInt32 DataSignature = 'MMDA';

	void OnSaveGame(const SKSESerializationInterface* intfc)
	{
		intfc->OpenRecord(DataSignature, PLUGIN_VERSION);
		SInt32 tInt = 0;
		intfc->WriteRecordData(&tInt, sizeof(SInt32));
	}

	void OnLoadGame(const SKSESerializationInterface* intfc)
	{
		UInt32 type, version, length;
		SInt32 tInt;
		intfc->GetNextRecordInfo(&type, &version, &length);
		if (type == DataSignature)
		{
			intfc->ReadRecordData(&tInt, sizeof(SInt32));
		}
	}

	bool RegisterSerialize(SKSESerializationInterface* g_serial, PluginHandle tHandle)
	{
		if (g_serial != nullptr)
		{
			//g_serial->SetUniqueID(tHandle, RecordSignature);
			//g_serial->SetSaveCallback(tHandle, OnSaveGame);
			//g_serial->SetLoadCallback(tHandle, OnLoadGame);
			return true;
		}

		return false;
	}
};