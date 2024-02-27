#pragma once

#include <rapidjson/document.h>

class ZBinarySerializer;

struct SVector3
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static SVector3* DeserializeFromJson(const rapidjson::Value& object);
	bool operator==(const SVector3& other) const;

	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		float v[3];
	};
};
