#pragma once
#include <crow.h>
#include "database.h"

class Routing
{
public:
	Routing();
	void run();
private:
	crow::SimpleApp m_app;
	Database db;
	};