#include "Glacier/Sound/ZSpeakerLevels.h"
#include "Glacier/Serializer/ZBinarySerializer.h"

const TArray<float>& ZSpeakerLevels::GetSpeakerMatrix() const
{
    return m_aSpeakerMatrix;
}

void ZSpeakerLevels::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
    unsigned int speakerMatrixOffset = offset + offsetof(ZSpeakerLevels, m_aSpeakerMatrix);

    m_aSpeakerMatrix.SerializeToMemory(binarySerializer, speakerMatrixOffset);
}

ZSpeakerLevels* ZSpeakerLevels::DeserializeFromJson(const rapidjson::Value& object)
{
    ZSpeakerLevels* speakerLevels = new ZSpeakerLevels();

    TArray<float>::DeserializeFromJson(speakerLevels->m_aSpeakerMatrix, object["m_aSpeakerMatrix"].GetArray());

    return speakerLevels;
}

bool ZSpeakerLevels::operator==(const ZSpeakerLevels& other) const
{
    return m_aSpeakerMatrix == other.m_aSpeakerMatrix;
}
