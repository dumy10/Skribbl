#include "routing.h"
#include<iostream>

import player;

using namespace skribbl;

Routing::Routing()
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ db.GetRandomWord() };
		});

}

void Routing::run()
{
	std::cout << "It is running";
	m_app.port(18080).multithreaded().run();
}
