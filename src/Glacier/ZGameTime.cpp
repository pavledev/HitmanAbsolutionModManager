#include "Glacier/ZGameTime.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

long long ZGameTime::GetTicks() const
{
    return m_nTicks;
}

void ZGameTime::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int ticksOffset = offset + offsetof(ZGameTime, m_nTicks);

    binarySerializer.WriteToMemory(&m_nTicks, sizeof(long long), ticksOffset);
}

void ZGameTime::DeserializeFromJson(ZGameTime& gameTime, const rapidjson::Value& object)
{
    gameTime.m_nTicks = object["m_nTicks"].GetInt64();
}

ZGameTime* ZGameTime::DeserializeFromJson(const rapidjson::Value& object)
{
    ZGameTime* gameTime = new ZGameTime();

    DeserializeFromJson(*gameTime, object);

    return gameTime;
}

bool ZGameTime::operator==(const ZGameTime& other) const
{
    return m_nTicks == other.m_nTicks;
}
