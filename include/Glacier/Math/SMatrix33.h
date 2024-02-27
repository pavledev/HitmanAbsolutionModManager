#pragma once

#include <rapidjson/document.h>

#include "SVector3.h"

struct SMatrix33
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static SMatrix33* DeserializeFromJson(const rapidjson::Value& object);
	bool operator==(const SMatrix33& other) const;

	union
	{
		struct
		{
			SVector3 XAxis;
			SVector3 YAxis;
			SVector3 ZAxis;
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
		};

		float v[9];
		SVector3 r[3];
	};
};
