#pragma once

#include <iostream>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "../Containers/TArray.h"
#include "../TypeInfo/STypeID.h"

struct SEntityTemplateEntitySubset
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(SEntityTemplateEntitySubset& entityTemplateEntitySubset, const rapidjson::Value& object);

	STypeID* subsetType;
	TArray<int> entities;
};
