#pragma once

#include <rapidjson/document.h>

class ZBinarySerializer;

struct SVector2
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(SVector2& vector, const rapidjson::Value& object);
	static SVector2* DeserializeFromJson(const rapidjson::Value& object);
	bool operator==(const SVector2& other) const;

	union
	{
		struct
		{
			float x;
			float y;
		};

		float v[2];
	};
};
