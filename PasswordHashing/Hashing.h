#pragma once
#include "LibDefine.h"
#include <string>

class DLL_API Hasher
{
public:
	Hasher() = delete;
	Hasher(const Hasher&) = delete;
	static std::string HashPassword(const std::string& password);
};