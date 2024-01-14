#pragma once
#include "LibDefine.h"
#include <string>

class DLL_API Hasher
{
public:
	Hasher() = delete;
	Hasher(const Hasher&) = delete;
	static void HashPassword(const char* password);
};