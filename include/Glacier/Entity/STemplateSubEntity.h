#pragma once

#include "../Containers/TArray.h"
#include "SEntityTemplateProperty.h"

struct STemplateSubEntity
{
	STemplateSubEntity();
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(STemplateSubEntity& templateSubEntity, const rapidjson::Value& object);

	int parentIndex;
	int entityTypeResourceIndex;
	TArray<SEntityTemplateProperty> propertyValues;
	TArray<SEntityTemplateProperty> postInitPropertyValues;
};
