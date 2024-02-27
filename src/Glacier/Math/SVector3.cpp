#include "Glacier/Math/SVector3.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void SVector3::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int xOffset = offset + offsetof(SVector3, x);
    unsigned int yOffset = offset + offsetof(SVector3, y);
    unsigned int zOffset = offset + offsetof(SVector3, z);

    binarySerializer.WriteToMemory(&x, sizeof(float), xOffset);
    binarySerializer.WriteToMemory(&y, sizeof(float), yOffset);
    binarySerializer.WriteToMemory(&z, sizeof(float), zOffset);
}

SVector3* SVector3::DeserializeFromJson(const rapidjson::Value& object)
{
    SVector3* vector = new SVector3();

    vector->x = object["x"].GetFloat();
    vector->y = object["y"].GetFloat();
    vector->z = object["z"].GetFloat();

    return vector;
}

bool SVector3::operator==(const SVector3& other) const
{
    return x == other.x && y == other.y && z == other.z;
}
