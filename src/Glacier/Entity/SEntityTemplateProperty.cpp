#include <Windows.h>
#include <format>

#include "Glacier/Entity/SEntityTemplateProperty.h"
#include "Glacier/Serializer/ZBinarySerializer.h"
#include "Hash.h"

void SEntityTemplateProperty::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int nPropertyIDOffset = offset + offsetof(SEntityTemplateProperty, nPropertyID);
    unsigned int valueOffset = offset + offsetof(SEntityTemplateProperty, value);

    binarySerializer.WriteToMemory(&nPropertyID, sizeof(unsigned int), nPropertyIDOffset);

    value.SerializeToMemory(binarySerializer, valueOffset);
}

void SEntityTemplateProperty::DeserializeFromJson(SEntityTemplateProperty& entityTemplateProperty, const rapidjson::Value& object)
{
    entityTemplateProperty.nPropertyID = Hash::Crc32(object["propertyName"].GetString());

    ZVariant::DeserializeFromJson(entityTemplateProperty.value, object["value"].GetObj());
}
