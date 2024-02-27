#pragma once

#include <iostream>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "../ZString.h"

class ZBinarySerializer;

struct SEntityTemplatePinConnection
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(SEntityTemplatePinConnection& entityTemplatePinConnection, const rapidjson::Value& object);

	int fromID;
	int toID;
	ZString fromPinName;
	ZString toPinName;
};
