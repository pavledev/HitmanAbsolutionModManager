#include "Glacier/Math/SMatrix43.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void SMatrix43::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int rotOffset = offset + offsetof(SMatrix43, Rot);
    unsigned int transOffset = offset + offsetof(SMatrix43, Trans);

    Rot.SerializeToMemory(binarySerializer, rotOffset);
    Trans.SerializeToMemory(binarySerializer, transOffset);
}

SMatrix43* SMatrix43::DeserializeFromJson(const rapidjson::Value& object)
{
    SMatrix43* matrix = new SMatrix43();

    matrix->Rot = *SMatrix33::DeserializeFromJson(object["Rot"].GetObj());
    matrix->Trans = *SVector3::DeserializeFromJson(object["Trans"].GetObj());

    return matrix;
}

bool SMatrix43::operator==(const SMatrix43& other) const
{
    return Rot == other.Rot && Trans == other.Trans;
}
