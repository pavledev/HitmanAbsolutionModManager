#include "Glacier/SSettingsParamMultiplier.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void SSettingsParamMultiplier::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
	unsigned int settingsParamOffset = offset + offsetof(SSettingsParamMultiplier, m_eSettingsParam);
	unsigned int multiplierOffset = offset + offsetof(SSettingsParamMultiplier, m_fMultiplier);

	binarySerializer.WriteToMemory(&m_eSettingsParam, sizeof(unsigned int), settingsParamOffset);
	binarySerializer.WriteToMemory(&m_fMultiplier, sizeof(float), multiplierOffset);
}

void SSettingsParamMultiplier::DeserializeFromJson(SSettingsParamMultiplier& settingsParamMultiplier, const rapidjson::Value& object)
{
	settingsParamMultiplier.m_eSettingsParam = static_cast<ESettingsParam>(object["m_eSettingsParam"].GetUint());
	settingsParamMultiplier.m_fMultiplier = object["m_fMultiplier"].GetFloat();
}

SSettingsParamMultiplier* SSettingsParamMultiplier::DeserializeFromJson(const rapidjson::Value& object)
{
	SSettingsParamMultiplier* settingsParamMultiplier = new SSettingsParamMultiplier();

	DeserializeFromJson(*settingsParamMultiplier, object);

	return settingsParamMultiplier;
}
