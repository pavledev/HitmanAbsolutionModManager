#include "Glacier/Entity/STemplateEntityBlueprint.h"

void STemplateEntityBlueprint::SerializeToMemory(ZBinarySerializer& binarySerializer)
{
    unsigned int rootEntityIndexOffset = offsetof(STemplateEntityBlueprint, rootEntityIndex);
    unsigned int entityTemplatesOffset = offsetof(STemplateEntityBlueprint, entityTemplates);
    unsigned int pinConnectionsOffset = offsetof(STemplateEntityBlueprint, pinConnections);
    unsigned int inputPinForwardingsOffset = offsetof(STemplateEntityBlueprint, inputPinForwardings);
    unsigned int outputPinForwardingsOffset = offsetof(STemplateEntityBlueprint, outputPinForwardings);

    binarySerializer.WriteToMemory(&rootEntityIndex, sizeof(int), rootEntityIndexOffset);

    entityTemplates.SerializeToMemory(binarySerializer, entityTemplatesOffset);
    pinConnections.SerializeToMemory(binarySerializer, pinConnectionsOffset);
    inputPinForwardings.SerializeToMemory(binarySerializer, inputPinForwardingsOffset);
    outputPinForwardings.SerializeToMemory(binarySerializer, outputPinForwardingsOffset);
}

STemplateEntityBlueprint* STemplateEntityBlueprint::DeserializeFromJson(const rapidjson::Document& document)
{
    STemplateEntityBlueprint* templateEntityBlueprint = new STemplateEntityBlueprint();

    templateEntityBlueprint->rootEntityIndex = document["rootEntityIndex"].GetInt();

    TArray<STemplateSubEntityBlueprint>::DeserializeFromJson(templateEntityBlueprint->entityTemplates, document["entityTemplates"].GetArray());
    TArray<SEntityTemplatePinConnection>::DeserializeFromJson(templateEntityBlueprint->pinConnections, document["pinConnections"].GetArray());
    TArray<SEntityTemplatePinConnection>::DeserializeFromJson(templateEntityBlueprint->inputPinForwardings, document["inputPinForwardings"].GetArray());
    TArray<SEntityTemplatePinConnection>::DeserializeFromJson(templateEntityBlueprint->outputPinForwardings, document["outputPinForwardings"].GetArray());

    return templateEntityBlueprint;
}
