#pragma once

#include <string>

class StringUtility
{
public:
	static const char* FromChar8T(const char8_t* c)
	{
		return reinterpret_cast<const char*>(c);
	}

	static std::string ToUpperCase(const std::string& string);
	static std::string ToLowerCase(const std::string& string);
};
