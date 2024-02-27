#pragma once

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

#include "ESettingsParam.h"

class ZBinarySerializer;

struct SSettingsParamMultiplier
{
    void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
    static void DeserializeFromJson(SSettingsParamMultiplier& settingsParamMultiplier, const rapidjson::Value& object);
    static SSettingsParamMultiplier* DeserializeFromJson(const rapidjson::Value& object);

    ESettingsParam m_eSettingsParam;
    float m_fMultiplier;
};
