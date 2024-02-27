#include "Glacier/Entity/SEntityTemplateReference.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void SEntityTemplateReference::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int entityIndexOffset = offset + offsetof(SEntityTemplateReference, entityIndex);
    unsigned int exposedEntityOffset = offset + offsetof(SEntityTemplateReference, exposedEntity);

    binarySerializer.WriteToMemory(&entityIndex, sizeof(int), entityIndexOffset);

    exposedEntity.SerializeToMemory(binarySerializer, exposedEntityOffset);
}

void SEntityTemplateReference::DeserializeFromJson(SEntityTemplateReference& entityTemplateReference, const rapidjson::Value& object)
{
    entityTemplateReference.entityIndex = object["entityIndex"].GetInt();

    ZString::DeserializeFromJson(entityTemplateReference.exposedEntity, object["exposedEntity"].GetString());
}

SEntityTemplateReference* SEntityTemplateReference::DeserializeFromJson(const rapidjson::Value& object)
{
    SEntityTemplateReference* entityTemplateReference = new SEntityTemplateReference();

    DeserializeFromJson(*entityTemplateReference, object);

    return entityTemplateReference;
}

bool SEntityTemplateReference::operator==(const SEntityTemplateReference& other)
{
    return entityIndex == other.entityIndex && exposedEntity == other.exposedEntity;
}
