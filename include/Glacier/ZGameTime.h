#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

class ZBinarySerializer;

class ZGameTime
{
public:
	long long GetTicks() const;
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(ZGameTime& gameTime, const rapidjson::Value& object);
	static ZGameTime* DeserializeFromJson(const rapidjson::Value& object);
	bool operator==(const ZGameTime& other) const;

private:
	long long m_nTicks;
};
