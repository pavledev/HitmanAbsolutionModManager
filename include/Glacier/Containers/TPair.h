#pragma once

#include <rapidjson/document.h>

class ZString;

template <typename TKey, typename TValue>
class TPair
{
public:
	TPair() = default;

	TPair(TKey m_key, TValue m_value)
	{
		this->m_key = m_key;
		this->m_value = m_value;
	}

	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
	{
		unsigned int keyOffset = offset + offsetof(TPair, m_key);
		unsigned int valueOffset = offset + offsetof(TPair, m_value);

		if constexpr (std::is_class_v<TKey>)
		{
			m_key.SerializeToMemory(binarySerializer, keyOffset);
		}
		else
		{
			binarySerializer.WriteToMemory(&m_key, sizeof(TKey), keyOffset);
		}

		if constexpr (std::is_class_v<TValue>)
		{
			m_value.SerializeToMemory(binarySerializer, valueOffset);
		}
		else
		{
			binarySerializer.WriteToMemory(&m_value, sizeof(TValue), valueOffset);
		}
	}

	static void DeserializeFromJson(TPair& pair, const rapidjson::Value& object)
	{
		if constexpr (std::is_class_v<TKey>)
		{
			if constexpr (std::is_same_v<TKey, ZString>)
			{
				TKey::DeserializeFromJson(pair.m_key, object["m_key"].GetString());
			}
			else
			{
				TKey::DeserializeFromJson(pair.m_key, object["m_key"].GetObject());
			}
		}
		else
		{
			if constexpr (std::is_same_v<TKey, char> || std::is_same_v<TKey, short> || std::is_same_v<TKey, int> || std::is_enum_v<TKey>)
			{
				pair.m_key = object["m_key"].GetInt();
			}
			else if constexpr (std::is_same_v<TKey, unsigned char> || std::is_same_v<TKey, unsigned short> || std::is_same_v<TKey, unsigned int>)
			{
				pair.m_key = object["m_key"].GetUint();
			}
			else if constexpr (std::is_same_v<TKey, long long>)
			{
				pair.m_key = object["m_key"].GetInt64();
			}
			else if constexpr (std::is_same_v<TKey, unsigned long long>)
			{
				pair.m_key = object["m_key"].GetUint64();
			}
			else if constexpr (std::is_same_v<TKey, float>)
			{
				pair.m_key = object["m_key"].GetFloat();
			}
			else if constexpr (std::is_same_v<TKey, double>)
			{
				pair.m_key = object["m_key"].GetDouble();
			}
			else if constexpr (std::is_same_v<TKey, bool>)
			{
				pair.m_key = object["m_key"].GetBool();
			}
		}

		if constexpr (std::is_class_v<TValue>)
		{
			TValue::DeserializeFromJson(pair.m_value, object["m_value"].GetObject());
		}
		else
		{
			if constexpr (std::is_same_v<TValue, char> || std::is_same_v<TValue, short> || std::is_same_v<TValue, int> || std::is_enum_v<TValue>)
			{
				pair.m_value = object["m_value"].GetInt();
			}
			else if constexpr (std::is_same_v<TValue, unsigned char> || std::is_same_v<TValue, unsigned short> || std::is_same_v<TValue, unsigned int>)
			{
				pair.m_value = object["m_value"].GetUint();
			}
			else if constexpr (std::is_same_v<TValue, long long>)
			{
				pair.m_value = object["m_value"].GetInt64();
			}
			else if constexpr (std::is_same_v<TValue, unsigned long long>)
			{
				pair.m_value = object["m_value"].GetUint64();
			}
			else if constexpr (std::is_same_v<TValue, float>)
			{
				pair.m_value = object["m_value"].GetFloat();
			}
			else if constexpr (std::is_same_v<TValue, double>)
			{
				pair.m_value = object["m_value"].GetDouble();
			}
			else if constexpr (std::is_same_v<TValue, bool>)
			{
				pair.m_value = object["m_value"].GetBool();
			}
		}
	}

private:
	TKey m_key;
	TValue m_value;
};
