#pragma once

#include <rapidjson/document.h>

#include "TFixedArrayBase.h"
#include "ZFixedArrayData.h"

template <typename TElement, unsigned int TSize>
class TFixedArray : public TFixedArrayBase<TElement, ZFixedArrayData<TElement, TSize>>
{
public:
    unsigned int Size() const
    {
        return TSize;
    }

    TElement& operator[](unsigned int nIndex)
    {
        return this->m_pStart[nIndex];
    }

    const TElement& operator[](unsigned int nIndex) const
    {
        return this->m_pStart[nIndex];
    }

    void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int elementsCount = Size();

        for (unsigned int i = 0; i < elementsCount; ++i)
        {
            unsigned int elementOffset = offset + sizeof(TElement) * i;
            TElement element = this->operator[](i);

            if constexpr (std::is_class_v<TElement>)
            {
                element.SerializeToMemory(binarySerializer, elementOffset);
            }
            else
            {
                binarySerializer.WriteToMemory(&element, sizeof(TElement), elementOffset);
            }
        }
    }

    static void DeserializeFromJson(TFixedArray<TElement, TSize>& fixedArray, const rapidjson::Value& jsonArray)
    {
        unsigned int index = 0;

        for (rapidjson::Value::ConstValueIterator it = jsonArray.Begin(); it != jsonArray.End(); ++it)
        {
            if constexpr (std::is_class_v<TElement>)
            {
                const rapidjson::Value& object2 = it->GetObj();
                TElement element = TElement::DeserializeFromJson(object2);

                fixedArray[index] = element;
            }
            else
            {
                if constexpr (std::is_same_v<TElement, char> || std::is_same_v<TElement, short> || std::is_same_v<TElement, int> || std::is_enum_v<TElement>)
                {
                    fixedArray[index] = it->GetInt();
                }
                else if constexpr (std::is_same_v<TElement, unsigned char> || std::is_same_v<TElement, unsigned short> || std::is_same_v<TElement, unsigned int>)
                {
                    fixedArray[index] = it->GetUint();
                }
                else if constexpr (std::is_same_v<TElement, long long>)
                {
                    fixedArray[index] = it->GetInt64();
                }
                else if constexpr (std::is_same_v<TElement, unsigned long long>)
                {
                    fixedArray[index] = it->GetUint64();
                }
                else if constexpr (std::is_same_v<TElement, float>)
                {
                    fixedArray[index] = it->GetFloat();
                }
                else if constexpr (std::is_same_v<TElement, double>)
                {
                    fixedArray[index] = it->GetDouble();
                }
                else if constexpr (std::is_same_v<TElement, bool>)
                {
                    fixedArray[index] = it->GetBool();
                }
            }

            index++;
        }
    }

    bool operator==(const TFixedArray& other) const
    {
        for (unsigned int i = 0; i < Size(); ++i)
        {
            if (this->operator[](i) != other[i])
            {
                return false;
            }
        }

        return true;
    }
};
