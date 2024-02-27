#include "Glacier/Entity/SEntityTemplatePinConnection.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void SEntityTemplatePinConnection::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int fromIDOffset = offset + offsetof(SEntityTemplatePinConnection, fromID);
    unsigned int toIDOffset = offset + offsetof(SEntityTemplatePinConnection, toID);
    unsigned int fromPinNameOffset = offset + offsetof(SEntityTemplatePinConnection, fromPinName);
    unsigned int toPinNameOffset = offset + offsetof(SEntityTemplatePinConnection, toPinName);

    binarySerializer.WriteToMemory(&fromID, sizeof(int), fromIDOffset);
    binarySerializer.WriteToMemory(&toID, sizeof(int), toIDOffset);

    fromPinName.SerializeToMemory(binarySerializer, fromPinNameOffset);
    toPinName.SerializeToMemory(binarySerializer, toPinNameOffset);
}

void SEntityTemplatePinConnection::DeserializeFromJson(SEntityTemplatePinConnection& entityTemplatePinConnection, const rapidjson::Value& object)
{
    entityTemplatePinConnection.fromID = object["fromID"].GetInt();
    entityTemplatePinConnection.toID = object["toID"].GetInt();

    ZString::DeserializeFromJson(entityTemplatePinConnection.fromPinName, object["fromPinName"].GetString());
    ZString::DeserializeFromJson(entityTemplatePinConnection.toPinName, object["toPinName"].GetString());
}
