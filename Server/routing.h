#pragma once
#include <crow.h>
#include "database.h"
import game;

class Routing
{
public:
	void run(Database& storage);
private:
	crow::SimpleApp m_app;
	Game m_game;
	};