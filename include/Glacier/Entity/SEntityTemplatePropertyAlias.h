#pragma once

#include <iostream>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "../ZString.h"
#include "../Serializer/ZBinarySerializer.h"

struct SEntityTemplatePropertyAlias
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(SEntityTemplatePropertyAlias& entityTemplatePropertyAlias, const rapidjson::Value& object);

	ZString sAliasName;
	int entityID;
	ZString sPropertyName;
};
