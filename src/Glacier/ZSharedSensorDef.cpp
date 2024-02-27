#include "Glacier/ZSharedSensorDef.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

void ZSharedSensorDef::SVisibilitySetting::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int sensitivityOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_eSensitivity);
    unsigned int closeRangeOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fCloseRange);
    unsigned int peripheralRangeOffsetOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fPeripheralRange);
    unsigned int peripheralAngleOffsetOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fPeripheralAngle);
    unsigned int peripheralHighOffsetOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fPeripheralHigh);
    unsigned int peripheralLowOffsetOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fPeripheralLow);
    unsigned int focusConeWidthAngleOffsetOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fFocusConeWidthAngle);
    unsigned int focusConeHeightAngleOffsetOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fFocusConeHeightAngle);
    unsigned int focusConeRangeOffset = offset + offsetof(ZSharedSensorDef::SVisibilitySetting, m_fFocusConeRange);

    binarySerializer.WriteToMemory(&m_eSensitivity, sizeof(unsigned int), sensitivityOffset);
    binarySerializer.WriteToMemory(&m_fCloseRange, sizeof(float), closeRangeOffset);
    binarySerializer.WriteToMemory(&m_fPeripheralRange, sizeof(float), peripheralRangeOffsetOffset);
    binarySerializer.WriteToMemory(&m_fPeripheralAngle, sizeof(float), peripheralAngleOffsetOffset);
    binarySerializer.WriteToMemory(&m_fPeripheralHigh, sizeof(float), peripheralHighOffsetOffset);
    binarySerializer.WriteToMemory(&m_fPeripheralLow, sizeof(float), peripheralLowOffsetOffset);
    binarySerializer.WriteToMemory(&m_fFocusConeWidthAngle, sizeof(float), focusConeWidthAngleOffsetOffset);
    binarySerializer.WriteToMemory(&m_fFocusConeHeightAngle, sizeof(float), focusConeHeightAngleOffsetOffset);
    binarySerializer.WriteToMemory(&m_fFocusConeRange, sizeof(float), focusConeRangeOffset);
}

void ZSharedSensorDef::SVisibilitySetting::DeserializeFromJson(SVisibilitySetting& visibilitySetting, const rapidjson::Value& object)
{
    visibilitySetting.m_eSensitivity = static_cast<EActorPerceptionSensitivity>(object["m_eSensitivity"].GetUint());
    visibilitySetting.m_fCloseRange = object["m_fCloseRange"].GetFloat();
    visibilitySetting.m_fPeripheralRange = object["m_fPeripheralRange"].GetFloat();
    visibilitySetting.m_fPeripheralAngle = object["m_fPeripheralAngle"].GetFloat();
    visibilitySetting.m_fPeripheralHigh = object["m_fPeripheralHigh"].GetFloat();
    visibilitySetting.m_fPeripheralLow = object["m_fPeripheralLow"].GetFloat();
    visibilitySetting.m_fFocusConeWidthAngle = object["m_fFocusConeWidthAngle"].GetFloat();
    visibilitySetting.m_fFocusConeHeightAngle = object["m_fFocusConeHeightAngle"].GetFloat();
    visibilitySetting.m_fFocusConeRange = object["m_fFocusConeRange"].GetFloat();
}

ZSharedSensorDef::SVisibilitySetting* ZSharedSensorDef::SVisibilitySetting::DeserializeFromJson(const rapidjson::Value& object)
{
    SVisibilitySetting* visibilitySetting = new SVisibilitySetting();

    DeserializeFromJson(*visibilitySetting, object);

    return visibilitySetting;
}

bool ZSharedSensorDef::SVisibilitySetting::operator==(const SVisibilitySetting& other)
{
    return m_eSensitivity == other.m_eSensitivity &&
        m_fCloseRange == other.m_fCloseRange &&
        m_fPeripheralRange == other.m_fPeripheralRange &&
        m_fPeripheralAngle == other.m_fPeripheralAngle &&
        m_fPeripheralHigh == other.m_fPeripheralHigh &&
        m_fPeripheralLow == other.m_fPeripheralLow &&
        m_fFocusConeWidthAngle == other.m_fFocusConeWidthAngle &&
        m_fFocusConeHeightAngle == other.m_fFocusConeHeightAngle &&
        m_fFocusConeRange == other.m_fFocusConeRange;
}
