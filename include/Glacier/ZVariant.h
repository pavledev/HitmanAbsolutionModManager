#pragma once

#include <unordered_map>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

#include "ZObjectRef.h"

class ZBinarySerializer;

class ZVariant : public ZObjectRef
{
public:
	ZVariant();
	~ZVariant();
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(ZVariant& variant, const rapidjson::Value& object);
	static void* GetValue(const ZVariant& variant, const std::unordered_map<STypeID*, std::vector<void*>>& values);
};
