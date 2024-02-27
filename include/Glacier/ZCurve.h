#pragma once

#include "Containers/TArray.h"
#include "Containers/TFixedArray.h"

class ZBinarySerializer;

class ZCurve
{
public:
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static ZCurve* DeserializeFromJson(const rapidjson::Value& object);
	bool operator==(const ZCurve& other) const;

private:
	TArray<TFixedArray<float, 8>> data;
};
