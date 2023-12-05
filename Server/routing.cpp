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
    CROW_ROUTE(m_app, "/joinGame")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		//m_game.addPlayer() depends on how we modify the classes
		return crow::response{ "You joined the game!" };
		});
	CROW_ROUTE(m_app, "/endGame")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		m_game.endGame();
		return crow::response{ "Game ended!" };
		});
    CROW_ROUTE(m_app, "/leaderboard")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		//m_game.leaderboard();
		return crow::response{ "The winner is: " };
		});
	CROW_ROUTE(m_app, "/modifyRound")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		//m_game.modifyRound();
		return crow::response{ "Round modified!" };
		});
	CROW_ROUTE(m_app, "/removePlayer")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		//m_game.removePlayer();
		return crow::response{ "Player removed!" };
		});
	CROW_ROUTE(m_app, "/addAdmin")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		//m_game.addAdmin();
		return crow::response{ "Admin added!" };
		});
	CROW_ROUTE(m_app, "/removeAdmin")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		//m_game.removeAdmin();
		return crow::response{ "Admin removed!" };
		});
	m_app.port(18080).multithreaded().run();
}
