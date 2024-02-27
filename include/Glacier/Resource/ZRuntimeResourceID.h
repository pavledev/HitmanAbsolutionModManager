#pragma once

#include <string>

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

class ZBinarySerializer;

class ZRuntimeResourceID
{
public:
	ZRuntimeResourceID();
	ZRuntimeResourceID(unsigned long long runtimeResourceID);
	ZRuntimeResourceID(unsigned int idHigh, unsigned int idLow);
	operator long long() const;
	unsigned long long GetID() const;
	bool operator==(const ZRuntimeResourceID& other) const;
	bool operator!=(const ZRuntimeResourceID& other) const;
	static ZRuntimeResourceID QueryRuntimeResourceID(const std::string& idResource);
	void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	static ZRuntimeResourceID* DeserializeFromJson(const rapidjson::Value& object);

private:
	unsigned int m_IDHigh;
	unsigned int m_IDLow;
};
