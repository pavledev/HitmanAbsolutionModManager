#include "Glacier/Entity/STemplateSubEntityBlueprint.h"

void STemplateSubEntityBlueprint::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int parentIndexOffset = offset + offsetof(STemplateSubEntityBlueprint, parentIndex);
    unsigned int entityTypeResourceIndexOffset = offset + offsetof(STemplateSubEntityBlueprint, entityTypeResourceIndex);
    unsigned int entityNameOffset = offset + offsetof(STemplateSubEntityBlueprint, entityName);
    unsigned int propertyAliasesOffset = offset + offsetof(STemplateSubEntityBlueprint, propertyAliases);
    unsigned int exposedEntitiesOffset = offset + offsetof(STemplateSubEntityBlueprint, exposedEntities);
    unsigned int exposedInterfacesOffset = offset + offsetof(STemplateSubEntityBlueprint, exposedInterfaces);
    unsigned int entitySubsetsOffset = offset + offsetof(STemplateSubEntityBlueprint, entitySubsets);

    binarySerializer.WriteToMemory(&parentIndex, sizeof(int), parentIndexOffset);
    binarySerializer.WriteToMemory(&entityTypeResourceIndex, sizeof(int), entityTypeResourceIndexOffset);

    entityName.SerializeToMemory(binarySerializer, entityNameOffset);
    propertyAliases.SerializeToMemory(binarySerializer, propertyAliasesOffset);
    exposedEntities.SerializeToMemory(binarySerializer, exposedEntitiesOffset);
    exposedInterfaces.SerializeToMemory(binarySerializer, exposedInterfacesOffset);
    entitySubsets.SerializeToMemory(binarySerializer, entitySubsetsOffset);
}

void STemplateSubEntityBlueprint::DeserializeFromJson(STemplateSubEntityBlueprint& templateSubEntityBlueprint, const rapidjson::Value& object)
{
    templateSubEntityBlueprint.parentIndex = object["parentIndex"].GetInt();
    templateSubEntityBlueprint.entityTypeResourceIndex = object["tbluEntityTypeResourceIndex"].GetInt();

    ZString::DeserializeFromJson(templateSubEntityBlueprint.entityName, object["entityName"].GetString());
    TArray<SEntityTemplatePropertyAlias>::DeserializeFromJson(templateSubEntityBlueprint.propertyAliases, object["propertyAliases"].GetArray());
    TArray<TPair<ZString, SEntityTemplateReference>>::DeserializeFromJson(templateSubEntityBlueprint.exposedEntities, object["exposedEntities"].GetArray());
    TArray<TPair<ZString, int>>::DeserializeFromJson(templateSubEntityBlueprint.exposedInterfaces, object["exposedInterfaces"].GetArray());
    TArray<TPair<ZString, SEntityTemplateEntitySubset>>::DeserializeFromJson(templateSubEntityBlueprint.entitySubsets, object["entitySubsets"].GetArray());
}
