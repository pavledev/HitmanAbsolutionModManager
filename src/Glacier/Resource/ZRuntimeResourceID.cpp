#include <format>
#include <Windows.h>

#include "Glacier/Resource/ZRuntimeResourceID.h"
#include "Glacier/Serializer/ZBinarySerializer.h"
#include "Registry/ResourceIDRegistry.h"

ZRuntimeResourceID::ZRuntimeResourceID()
{
	m_IDHigh = 0;
	m_IDLow = 0;
}

ZRuntimeResourceID::ZRuntimeResourceID(unsigned long long runtimeResourceID)
{
	m_IDHigh = runtimeResourceID >> 32;
	m_IDLow = static_cast<unsigned int>(runtimeResourceID);
}

ZRuntimeResourceID::ZRuntimeResourceID(unsigned int idHigh, unsigned int idLow)
{
	m_IDHigh = idHigh;
	m_IDLow = idLow;
}

ZRuntimeResourceID::operator long long() const
{
	return GetID();
}

unsigned long long ZRuntimeResourceID::GetID() const
{
	return (static_cast<unsigned long long>(m_IDHigh) << 32) | m_IDLow;
}

ZRuntimeResourceID ZRuntimeResourceID::QueryRuntimeResourceID(const std::string& idResource)
{
	while (!ResourceIDRegistry::GetInstance().IsLoaded())
	{
		Sleep(20);
	}

	return ResourceIDRegistry::GetInstance().GetRuntimeResourceID(idResource);
}

bool ZRuntimeResourceID::operator==(const ZRuntimeResourceID& other) const
{
	return GetID() == other.GetID();
}

bool ZRuntimeResourceID::operator!=(const ZRuntimeResourceID& other) const
{
	return GetID() != other.GetID();
}

void ZRuntimeResourceID::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int idHighOffset = offset + offsetof(ZRuntimeResourceID, m_IDHigh);
    unsigned int idLowOffset = offset + offsetof(ZRuntimeResourceID, m_IDLow);

    binarySerializer.WriteToMemory(&m_IDHigh, sizeof(unsigned int), idHighOffset);
    binarySerializer.WriteToMemory(&m_IDLow, sizeof(unsigned int), idLowOffset);

    binarySerializer.RecordRuntimeResourceIDOffset(offset);
}

ZRuntimeResourceID* ZRuntimeResourceID::DeserializeFromJson(const rapidjson::Value& object)
{
	std::string resourceID = object["resourceID"].GetString();

	if (!resourceID.empty())
	{
		return new ZRuntimeResourceID(QueryRuntimeResourceID(resourceID));
	}

	return new ZRuntimeResourceID(-1);
}
