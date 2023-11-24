#include "routing.h"

import player;

using namespace skribbl;

Routing::Routing()
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ db.GetRandomWord() };
		});
	//both work fine
}

void Routing::run()
{
	m_app.port(18080).multithreaded().run();
}
