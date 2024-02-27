#include "Glacier/Entity/STemplateSubEntity.h"

STemplateSubEntity::STemplateSubEntity()
{
    parentIndex = 0;
    entityTypeResourceIndex = 0;
    propertyValues = TArray<SEntityTemplateProperty>();
    postInitPropertyValues = TArray<SEntityTemplateProperty>();
}

void STemplateSubEntity::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int parentIndexOffset = offset + offsetof(STemplateSubEntity, parentIndex);
    unsigned int entityTypeResourceIndexOffset = offset + offsetof(STemplateSubEntity, entityTypeResourceIndex);
    unsigned int propertyValuesOffset = offset + offsetof(STemplateSubEntity, propertyValues);
    unsigned int postInitPropertyValuesOffset = offset + offsetof(STemplateSubEntity, postInitPropertyValues);

    binarySerializer.WriteToMemory(&parentIndex, sizeof(int), parentIndexOffset);
    binarySerializer.WriteToMemory(&entityTypeResourceIndex, sizeof(int), entityTypeResourceIndexOffset);

    propertyValues.SerializeToMemory(binarySerializer, propertyValuesOffset);
    postInitPropertyValues.SerializeToMemory(binarySerializer, postInitPropertyValuesOffset);
}

void STemplateSubEntity::DeserializeFromJson(STemplateSubEntity& templateSubEntity, const rapidjson::Value& object)
{
    const rapidjson::Value& propertyValues = object["propertyValues"];
    const rapidjson::Value& postInitPropertyValues = object["propertyValues"];

    templateSubEntity.parentIndex = object["parentIndex"].GetInt();
    templateSubEntity.entityTypeResourceIndex = object["tempEntityTypeResourceIndex"].GetInt();

    TArray<SEntityTemplateProperty>::DeserializeFromJson(templateSubEntity.propertyValues, object["propertyValues"].GetArray());
    TArray<SEntityTemplateProperty>::DeserializeFromJson(templateSubEntity.postInitPropertyValues, object["postInitPropertyValues"].GetArray());
}
