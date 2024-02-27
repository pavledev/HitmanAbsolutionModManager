#include "Glacier/Entity/SEntityTemplateEntitySubset.h"

void SEntityTemplateEntitySubset::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int subsetTypeOffset = offset + offsetof(SEntityTemplateEntitySubset, subsetType);
    unsigned int entitiesOffset = offset + offsetof(SEntityTemplateEntitySubset, entities);

    subsetType->SerializeToMemory(binarySerializer, subsetTypeOffset);
    entities.SerializeToMemory(binarySerializer, entitiesOffset);
}

void SEntityTemplateEntitySubset::DeserializeFromJson(SEntityTemplateEntitySubset& entityTemplateEntitySubset, const rapidjson::Value& object)
{
    entityTemplateEntitySubset.subsetType = STypeID::DeserializeFromJson(object["subsetType"].GetString());

    TArray<int>::DeserializeFromJson(entityTemplateEntitySubset.entities, object["entities"].GetArray());
}
