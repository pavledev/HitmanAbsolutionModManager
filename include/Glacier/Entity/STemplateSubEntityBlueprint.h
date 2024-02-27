#pragma once

#include "../ZString.h"
#include "../Containers/TArray.h"
#include "../Containers/TPair.h"
#include "SEntityTemplatePropertyAlias.h"
#include "SEntityTemplateReference.h"
#include "SEntityTemplateEntitySubset.h"

struct STemplateSubEntityBlueprint
{
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static void DeserializeFromJson(STemplateSubEntityBlueprint& templateSubEntityBlueprint, const rapidjson::Value& object);

	int parentIndex;
	int entityTypeResourceIndex;
	ZString entityName;
	TArray<SEntityTemplatePropertyAlias> propertyAliases;
	TArray<TPair<ZString, SEntityTemplateReference>> exposedEntities;
	TArray<TPair<ZString, int>> exposedInterfaces;
	TArray<TPair<ZString, SEntityTemplateEntitySubset>> entitySubsets;
};
