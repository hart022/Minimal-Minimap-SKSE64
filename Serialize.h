#pragma once

#ifndef hSerialize
#define hSerialize

#include <SKSE/PluginAPI.h>

namespace Serialize
{
	extern UInt32 RecordSignature;
	extern UInt32 DataSignature;

	void OnSaveGame(const SKSESerializationInterface*);
	void OnLoadGame(const SKSESerializationInterface*);

	bool RegisterSerialize(SKSESerializationInterface*, PluginHandle);
};

#endif // !hSerialize

