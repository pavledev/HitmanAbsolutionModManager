#include "Glacier/ZCurve.h"

void ZCurve::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
	unsigned int dataOffset = offset + offsetof(ZCurve, data);

	data.SerializeToMemory(binarySerializer, dataOffset);
}

ZCurve* ZCurve::DeserializeFromJson(const rapidjson::Value& object)
{
	ZCurve* curve = new ZCurve();

	TArray<TFixedArray<float, 8>>::DeserializeFromJson(curve->data, object["data"].GetArray());

	return curve;
}

bool ZCurve::operator==(const ZCurve& other) const
{
	return data == other.data;
}
