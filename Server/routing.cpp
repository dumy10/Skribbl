#include "routing.h"

import player;

using namespace skribbl;



void Routing::run(Database& storage)
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ storage.GetRandomWord() };
		});
	CROW_ROUTE(m_app, "/startGame")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		m_game.startGame();
		return crow::response{ "Game started!" };
		});

	m_app.port(18080).multithreaded().run();
}
