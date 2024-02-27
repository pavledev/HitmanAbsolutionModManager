#pragma once

#include <iostream>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "../ZVariant.h"

class ZBinarySerializer;

struct SEntityTemplateProperty
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(SEntityTemplateProperty& entityTemplateProperty, const rapidjson::Value& object);

	unsigned int nPropertyID;
	ZVariant value;
};
