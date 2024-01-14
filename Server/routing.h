#pragma once
#include <crow.h>
#include "database.h"
#include "../PasswordHashing/Hashing.h"

class Routing
{
public:
	void Run(Database& storage);
private:
	crow::SimpleApp m_app;
};