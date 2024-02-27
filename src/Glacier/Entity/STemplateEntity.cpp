#include "Glacier/Entity/STemplateEntity.h"

void STemplateEntity::SerializeToMemory(ZBinarySerializer& binarySerializer)
{
    unsigned int blueprintIndexInResourceHeaderOffset = offsetof(STemplateEntity, blueprintIndexInResourceHeader);
    unsigned int rootEntityIndexOffset = offsetof(STemplateEntity, rootEntityIndex);
    unsigned int entityTemplatesOffset = offsetof(STemplateEntity, entityTemplates);

    binarySerializer.WriteToMemory(&blueprintIndexInResourceHeader, sizeof(int), blueprintIndexInResourceHeaderOffset);
    binarySerializer.WriteToMemory(&rootEntityIndex, sizeof(int), rootEntityIndexOffset);

    entityTemplates.SerializeToMemory(binarySerializer, entityTemplatesOffset);
}

STemplateEntity* STemplateEntity::DeserializeFromJson(const rapidjson::Document& document)
{
    STemplateEntity* templateEntity = new STemplateEntity();

    templateEntity->blueprintIndexInResourceHeader = document["blueprintIndexInResourceHeader"].GetInt();
    templateEntity->rootEntityIndex = document["rootEntityIndex"].GetInt();

    TArray<STemplateSubEntity>::DeserializeFromJson(templateEntity->entityTemplates, document["entityTemplates"].GetArray());

    return templateEntity;
}
