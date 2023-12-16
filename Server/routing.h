#pragma once
#include <crow.h>
#include "database.h"

class Routing
{
public:
	void Run(Database& storage);
private:
	crow::SimpleApp m_app;
};