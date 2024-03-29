#pragma once

#include <rapidjson/document.h>

#include "SVector3.h"
#include "SMatrix33.h"

class ZBinarySerializer;

struct SMatrix43
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static SMatrix43* DeserializeFromJson(const rapidjson::Value& object);
	bool operator==(const SMatrix43& other) const;

	union
	{
		struct
		{
			SVector3 XAxis;
			SVector3 YAxis;
			SVector3 ZAxis;
			SVector3 Trans;
		};

		struct
		{
			SMatrix33 Rot;
			SVector3 Trans;
		};

		struct
		{
			float m11;
			float m12;
			float m13;
			float m21;
			float m22;
			float m23;
			float m31;
			float m32;
			float m33;
			float m41;
			float m42;
			float m43;
		};

		float v[12];
		SVector3 r[4];
	};
};
