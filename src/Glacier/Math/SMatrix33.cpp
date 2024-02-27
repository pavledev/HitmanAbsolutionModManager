#include <cmath>

#include "Glacier/Math/SMatrix33.h"

void SMatrix33::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int xAxisOffset = offset + offsetof(SMatrix33, XAxis);
    unsigned int yAxisOffset = offset + offsetof(SMatrix33, YAxis);
    unsigned int zAxisOffset = offset + offsetof(SMatrix33, ZAxis);

    XAxis.SerializeToMemory(binarySerializer, xAxisOffset);
    YAxis.SerializeToMemory(binarySerializer, yAxisOffset);
    ZAxis.SerializeToMemory(binarySerializer, zAxisOffset);
}

SMatrix33* SMatrix33::DeserializeFromJson(const rapidjson::Value& object)
{
    SMatrix33* matrix = new SMatrix33();

    matrix->XAxis = *SVector3::DeserializeFromJson(object["XAxis"].GetObj());
    matrix->YAxis = *SVector3::DeserializeFromJson(object["YAxis"].GetObj());
    matrix->ZAxis = *SVector3::DeserializeFromJson(object["ZAxis"].GetObj());

    return matrix;
}

bool SMatrix33::operator==(const SMatrix33& other) const
{
    return XAxis == other.XAxis && YAxis == other.YAxis && ZAxis == other.ZAxis;
}
