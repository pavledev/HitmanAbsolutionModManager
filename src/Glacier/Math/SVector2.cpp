#include "Glacier/Math/SVector2.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void SVector2::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int xOffset = offset + offsetof(SVector2, x);
    unsigned int yOffset = offset + offsetof(SVector2, y);

    binarySerializer.WriteToMemory(&x, sizeof(float), xOffset);
    binarySerializer.WriteToMemory(&y, sizeof(float), yOffset);
}

void SVector2::DeserializeFromJson(SVector2& vector, const rapidjson::Value& object)
{
    vector.x = object["x"].GetFloat();
    vector.y = object["y"].GetFloat();
}

SVector2* SVector2::DeserializeFromJson(const rapidjson::Value& object)
{
    SVector2* vector = new SVector2();

    DeserializeFromJson(*vector, object);

    return vector;
}

bool SVector2::operator==(const SVector2& other) const
{
    return x == other.x && y == other.y;
}
