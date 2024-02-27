#include "Glacier/Entity/SEntityTemplatePropertyAlias.h"

void SEntityTemplatePropertyAlias::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int sAliasNameOffset = offset + offsetof(SEntityTemplatePropertyAlias, sAliasName);
    unsigned int entityIDOffset = offset + offsetof(SEntityTemplatePropertyAlias, entityID);
    unsigned int sPropertyNameOffset = offset + offsetof(SEntityTemplatePropertyAlias, sPropertyName);

    sAliasName.SerializeToMemory(binarySerializer, sAliasNameOffset);
    binarySerializer.WriteToMemory(&entityID, sizeof(int), entityIDOffset);
    sPropertyName.SerializeToMemory(binarySerializer, sPropertyNameOffset);
}

void SEntityTemplatePropertyAlias::DeserializeFromJson(SEntityTemplatePropertyAlias& entityTemplatePropertyAlias, const rapidjson::Value& object)
{
    ZString::DeserializeFromJson(entityTemplatePropertyAlias.sAliasName, object["sAliasName"].GetString());

    entityTemplatePropertyAlias.entityID = object["entityID"].GetInt();
    entityTemplatePropertyAlias.sPropertyName = ZString(object["sPropertyName"].GetString());
}
