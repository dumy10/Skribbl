#pragma once
#include <crow.h>
#include "database.h"

class Routing
{
public:
	void run(Database& storage);
private:
	crow::SimpleApp m_app;
};