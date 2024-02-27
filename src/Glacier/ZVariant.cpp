#include <format>

#include "Glacier/ZVariant.h"
#include "Glacier/Serializer/ZBinarySerializer.h"
#include "Glacier/Math/SVector2.h"
#include "Glacier/Math/SVector3.h"
#include "Glacier/Math/SMatrix43.h"
#include "Glacier/SColorRGB.h"
#include "Glacier/SColorRGBA.h"
#include "Glacier/Entity/SEntityTemplateReference.h"
#include "Glacier/Resource/ZRuntimeResourceID.h"
#include "Glacier/Containers/TArray.h"
#include "Glacier/ZCurve.h"
#include "Glacier/ZGameTime.h"
#include "Glacier/ZSharedSensorDef.h"
#include "Glacier/Sound/ZSpeakerLevels.h"
#include "Glacier/ECameraState.h"
#include "Glacier/SBodyPartDamageMultipliers.h"
#include "Glacier/SSettingsParamMultiplier.h"

#include "Registry/EnumRegistry.h"
#include "Registry/TypeRegistry.h"
#include "Global.h"

ZVariant::ZVariant() : ZObjectRef()
{

}

ZVariant::~ZVariant()
{
	propertyDataPointersToReferenceCounts[m_pData]--;

	const unsigned int referenceCount = propertyDataPointersToReferenceCounts[m_pData];

	if (referenceCount == 0)
	{
		if (m_TypeID->pTypeInfo->IsPrimitiveType() || m_TypeID->pTypeInfo->IsEnumType())
		{
			operator delete(m_pData);
		}
		else
		{
			switch (m_TypeID->typeNum)
			{
				case 12:
					delete static_cast<SVector2*>(m_pData);
					break;
				case 13:
					delete static_cast<SVector3*>(m_pData);
					break;
				case 14:
					delete static_cast<SMatrix43*>(m_pData);
					break;
				case 15:
					delete static_cast<SColorRGB*>(m_pData);
					break;
				case 16:
					delete static_cast<SColorRGBA*>(m_pData);
					break;
				case 17:
					delete static_cast<SEntityTemplateReference*>(m_pData);
					break;
				case 18:
					delete static_cast<SBodyPartDamageMultipliers*>(m_pData);
					break;
				case 19:
					delete static_cast<ZRuntimeResourceID*>(m_pData);
					break;
				case 20:
					delete static_cast<ZString*>(m_pData);
					break;
				case 21:
					delete static_cast<ZCurve*>(m_pData);
					break;
				case 22:
					delete static_cast<ZGameTime*>(m_pData);
					break;
				case 23:
					delete static_cast<ZSpeakerLevels*>(m_pData);
					break;
				case 25:
					delete static_cast<TArray<SEntityTemplateReference>*>(m_pData);
					break;
				case 26:
					delete static_cast<TArray<float>*>(m_pData);
					break;
				case 27:
					delete static_cast<TArray<ZGameTime>*>(m_pData);
					break;
				case 28:
					delete static_cast<TArray<SVector2>*>(m_pData);
					break;
				case 29:
					delete static_cast<TArray<bool>*>(m_pData);
					break;
				case 30:
					delete static_cast<TArray<ZSharedSensorDef::SVisibilitySetting>*>(m_pData);
					break;
				case 31:
					delete static_cast<TArray<ZString>*>(m_pData);
					break;
				case 32:
					delete static_cast<TArray<SSettingsParamMultiplier>*>(m_pData);
					break;
				case 33:
					delete static_cast<TArray<SColorRGB>*>(m_pData);
					break;
				case 34:
					delete static_cast<TArray<ECameraState>*>(m_pData);
					break;
			}
		}
	}
}

void ZVariant::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
{
	unsigned int typeIDOffset = offset + offsetof(ZVariant, m_TypeID);
	unsigned int dataOffset = offset + offsetof(ZVariant, m_pData);
	unsigned int nullPointer = -1;

	binarySerializer.RecordOffsetForTypeIDReindexing(typeIDOffset);

	if (m_TypeID)
	{
		unsigned int typeIndex = binarySerializer.TypeIdToIndex(m_TypeID);

		binarySerializer.WriteToMemory(&typeIndex, sizeof(unsigned int), typeIDOffset);

		if (m_pData)
		{
			std::optional<unsigned int> result = binarySerializer.GetDataOffset(this);
			unsigned int dataOffset2;

			if (result.has_value())
			{
				dataOffset2 = result.value();
			}
			else
			{
				unsigned int typeSize = m_TypeID->pTypeInfo->GetTypeSize();
				unsigned int typeAlignment = m_TypeID->pTypeInfo->GetTypeAlignment();

				dataOffset2 = binarySerializer.GetAlignedOffset(typeSize, typeAlignment);

				std::string typeName = m_TypeID->pTypeInfo->GetTypeName();

				if (m_TypeID->pTypeInfo->IsPrimitiveType() || m_TypeID->pTypeInfo->IsEnumType())
				{
					binarySerializer.WriteToMemory(m_pData, typeSize, dataOffset2);
				}
				else
				{
					switch (m_TypeID->typeNum)
					{
						case 12:
							static_cast<SVector2*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 13:
							static_cast<SVector3*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 14:
							static_cast<SMatrix43*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 15:
							static_cast<SColorRGB*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 16:
							static_cast<SColorRGBA*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 17:
							static_cast<SEntityTemplateReference*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 18:
							static_cast<SBodyPartDamageMultipliers*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 19:
							static_cast<ZRuntimeResourceID*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 20:
							static_cast<ZString*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 21:
							static_cast<ZCurve*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 22:
							static_cast<ZGameTime*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 23:
							static_cast<ZSpeakerLevels*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 25:
							static_cast<TArray<SEntityTemplateReference>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 26:
							static_cast<TArray<float>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 27:
							static_cast<TArray<ZGameTime>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 28:
							static_cast<TArray<SVector2>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 29:
							static_cast<TArray<bool>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 30:
							static_cast<TArray<ZSharedSensorDef::SVisibilitySetting>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 31:
							static_cast<TArray<ZString>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 32:
							static_cast<TArray<SSettingsParamMultiplier>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 33:
							static_cast<TArray<SColorRGB>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
						case 34:
							static_cast<TArray<ECameraState>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
							break;
					}
				}

				binarySerializer.SetDataOffset(this, dataOffset2);
			}

			binarySerializer.WriteToMemory(&dataOffset2, sizeof(unsigned int), dataOffset);
		}
		else
		{
			binarySerializer.WriteToMemory(&nullPointer, sizeof(unsigned int), dataOffset);
		}
	}
	else
	{
		binarySerializer.WriteToMemory(&nullPointer, sizeof(unsigned int), typeIDOffset);
		binarySerializer.WriteToMemory(&nullPointer, sizeof(unsigned int), dataOffset);
	}

	binarySerializer.RecordOffsetForRebasing(dataOffset);
}

void ZVariant::DeserializeFromJson(ZVariant& variant, const rapidjson::Value& object)
{
	std::string typeName = object["typeName"].GetString();
	static std::unordered_map<STypeID*, std::vector<void*>> values;
	bool isEnum = false;

	variant.m_TypeID = STypeID::DeserializeFromJson(object["typeName"].GetString());

	//Create type id for enums
	if (!variant.m_TypeID)
	{
		variant.m_TypeID = new STypeID();

		variant.m_TypeID->flags = 0;
		variant.m_TypeID->typeNum = -1;

		IType* type = new IType(4, 4, 8, typeName.c_str(), variant.m_TypeID);

		variant.m_TypeID->pTypeInfo = type;

		TypeRegistry::GetInstance().RegisterType(variant.m_TypeID);
	}

	switch (variant.m_TypeID->typeNum)
	{
		case 0:
		{
			bool value2 = object["value"].GetBool();

			variant.m_pData = operator new(sizeof(bool));

			memcpy(variant.m_pData, &value2, sizeof(bool));

			break;
		}
		case 1:
		case 2:
		{
			char value = object["value"].GetInt();

			variant.m_pData = operator new(sizeof(char));

			memcpy(variant.m_pData, &value, sizeof(char));

			break;
		}
		case 3:
		{
			short value = object["value"].GetInt();

			variant.m_pData = operator new(sizeof(short));

			memcpy(variant.m_pData, &value, sizeof(short));

			break;
		}
		case 4:
		{
			int value = object["value"].GetInt();

			variant.m_pData = operator new(sizeof(int));

			memcpy(variant.m_pData, &value, sizeof(int));

			break;
		}
		case 5:
		{
			long long value = object["value"].GetInt64();

			variant.m_pData = operator new(sizeof(long long));

			memcpy(variant.m_pData, &value, sizeof(long long));

			break;
		}
		case 6:
		{
			unsigned char value = object["value"].GetUint();

			variant.m_pData = operator new(sizeof(unsigned char));

			memcpy(variant.m_pData, &value, sizeof(unsigned char));

			break;
		}
		case 7:
		{
			unsigned short value = object["value"].GetUint();

			variant.m_pData = operator new(sizeof(unsigned short));

			memcpy(variant.m_pData, &value, sizeof(unsigned short));

			break;
		}
		case 8:
		{
			unsigned int value = object["value"].GetUint();

			variant.m_pData = operator new(sizeof(unsigned int));

			memcpy(variant.m_pData, &value, sizeof(unsigned int));

			break;
		}
		case 9:
		{
			unsigned long long value2 = object["value"].GetUint64();

			variant.m_pData = operator new(sizeof(unsigned long long));

			memcpy(variant.m_pData, &value2, sizeof(unsigned long long));

			break;
		}
		case 10:
		{
			float value2 = object["value"].GetFloat();

			variant.m_pData = operator new(sizeof(float));

			memcpy(variant.m_pData, &value2, sizeof(float));

			break;
		}
		case 11:
		{
			double value2 = object["value"].GetDouble();

			variant.m_pData = operator new(sizeof(double));

			memcpy(variant.m_pData, &value2, sizeof(double));

			break;
		}
		case 12:
			variant.m_pData = SVector2::DeserializeFromJson(object["value"].GetObj());
			break;
		case 13:
			variant.m_pData = SVector3::DeserializeFromJson(object["value"].GetObj());
			break;
		case 14:
			variant.m_pData = SMatrix43::DeserializeFromJson(object["value"].GetObj());
			break;
		case 15:
			variant.m_pData = SColorRGB::DeserializeFromJson(object["value"].GetObj());
			break;
		case 16:
			variant.m_pData = SColorRGBA::DeserializeFromJson(object["value"].GetObj());
			break;
		case 17:
			variant.m_pData = SEntityTemplateReference::DeserializeFromJson(object["value"].GetObj());
			break;
		case 18:
			variant.m_pData = SBodyPartDamageMultipliers::DeserializeFromJson(object["value"].GetObj());
			break;
		case 19:
			variant.m_pData = ZRuntimeResourceID::DeserializeFromJson(object["value"].GetObj());
			break;
		case 20:
			variant.m_pData = ZString::DeserializeFromJson(object["value"].GetString());
			break;
		case 21:
			variant.m_pData = ZCurve::DeserializeFromJson(object["value"].GetObj());
			break;
		case 22:
			variant.m_pData = ZGameTime::DeserializeFromJson(object["value"].GetObj());
			break;
		case 23:
			variant.m_pData = ZSpeakerLevels::DeserializeFromJson(object["value"].GetObj());
			break;
		case 25:
			variant.m_pData = TArray<SEntityTemplateReference>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 26:
			variant.m_pData = TArray<float>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 27:
			variant.m_pData = TArray<ZGameTime>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 28:
			variant.m_pData = TArray<SVector2>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 29:
			variant.m_pData = TArray<bool>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 30:
			variant.m_pData = TArray<ZSharedSensorDef::SVisibilitySetting>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 31:
			variant.m_pData = TArray<ZString>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 32:
			variant.m_pData = TArray<SSettingsParamMultiplier>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 33:
			variant.m_pData = TArray<SColorRGB>::DeserializeFromJson(object["value"].GetArray());
			break;
		case 34:
			variant.m_pData = TArray<ECameraState>::DeserializeFromJson(object["value"].GetArray());
			break;
		default:
		{
			std::string value = object["value"].GetString();
			const std::map<int, std::string>& items = EnumRegistry::GetInstance().GetEnum(typeName);

			variant.m_pData = operator new(sizeof(int));

			for (auto it = items.begin(); it != items.end(); it++)
			{
				if (it->second == value)
				{
					memcpy(variant.m_pData, &it->first, sizeof(int));

					break;
				}
			}

			isEnum = true;
		}
	}

	void* value = GetValue(variant, values);

	if (value)
	{
		variant.m_pData = value;
	}
	else
	{
		//SEntityTemplateReference objects, TArray and enums aren't duplicated
		if (typeName != "SEntityTemplateReference" &&
			!typeName.starts_with("TArray") &&
			/*!variant.m_TypeID->pTypeInfo->IsEnumType()*/
			!isEnum)
		{
			values[variant.m_TypeID].push_back(variant.m_pData);
		}
	}

	propertyDataPointersToReferenceCounts[variant.m_pData]++;
}

void* ZVariant::GetValue(const ZVariant& variant, const std::unordered_map<STypeID*, std::vector<void*>>& values)
{
	auto it = values.find(variant.m_TypeID);
	void* result = nullptr;

	if (it != values.end())
	{
		std::string typeName = variant.m_TypeID->pTypeInfo->GetTypeName();
		bool found = false;
		int index = 0;

		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			if ((typeName == "bool") &&
				*static_cast<bool*>(variant.m_pData) == *static_cast<bool*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "int8" || typeName == "char") &&
				*static_cast<char*>(variant.m_pData) == *static_cast<char*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "uint8") &&
				*static_cast<unsigned char*>(variant.m_pData) == *static_cast<unsigned char*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "int16") &&
				*static_cast<short*>(variant.m_pData) == *static_cast<short*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "uint16") &&
				*static_cast<unsigned short*>(variant.m_pData) == *static_cast<unsigned short*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "int32") &&
				*static_cast<int*>(variant.m_pData) == *static_cast<int*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "uint32") &&
				*static_cast<unsigned int*>(variant.m_pData) == *static_cast<unsigned int*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "int64") &&
				*static_cast<long long*>(variant.m_pData) == *static_cast<long long*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "uint64") &&
				*static_cast<unsigned long long*>(variant.m_pData) == *static_cast<unsigned long long*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "float32") &&
				*static_cast<float*>(variant.m_pData) == *static_cast<float*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "float64") &&
				*static_cast<double*>(variant.m_pData) == *static_cast<double*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "SVector2") &&
				*static_cast<SVector2*>(variant.m_pData) == *static_cast<SVector2*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "SVector3") &&
				*static_cast<SVector3*>(variant.m_pData) == *static_cast<SVector3*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "SMatrix43") &&
				*static_cast<SMatrix43*>(variant.m_pData) == *static_cast<SMatrix43*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "SColorRGB") &&
				*static_cast<SColorRGB*>(variant.m_pData) == *static_cast<SColorRGB*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "SColorRGBA") &&
				*static_cast<SColorRGBA*>(variant.m_pData) == *static_cast<SColorRGBA*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "SBodyPartDamageMultipliers") &&
				*static_cast<SBodyPartDamageMultipliers*>(variant.m_pData) == *static_cast<SBodyPartDamageMultipliers*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "ZRuntimeResourceID") &&
				*static_cast<ZRuntimeResourceID*>(variant.m_pData) == *static_cast<ZRuntimeResourceID*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "ZString") &&
				*static_cast<ZString*>(variant.m_pData) == *static_cast<ZString*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "ZCurve") &&
				*static_cast<ZCurve*>(variant.m_pData) == *static_cast<ZCurve*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "ZGameTime") &&
				*static_cast<ZGameTime*>(variant.m_pData) == *static_cast<ZGameTime*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
			else if ((typeName == "ZSpeakerLevels") &&
				*static_cast<ZSpeakerLevels*>(variant.m_pData) == *static_cast<ZSpeakerLevels*>(it->second[i]))
			{
				found = true;
				index = i;

				break;
			}
		}

		if (found)
		{
			result = it->second[index];
		}
	}

	return result;
}
