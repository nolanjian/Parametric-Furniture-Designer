#pragma once
#include "UtilsExport.h"
#include <string>

class UTILS_API IParamsConverter
{
public:
	static bool Decode(const std::string& src, std::string& target);
	static bool Encode(const std::string& src, std::string& target);
protected:
private:
};