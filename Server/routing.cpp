#include "routing.h"
import player;
import game;
using namespace skribbl;



void Routing::run(Database& storage)
{

	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ storage.GetRandomWord() };
		});

	CROW_ROUTE(m_app, "/checkUsername")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];

		if (username == "")
			return crow::response{ 404, "Username not found." };

		if (storage.CheckUsername(username))
			return crow::response{ "true" };

		return crow::response{ 409, "Username does not exist!" };
			});

	CROW_ROUTE(m_app, "/addUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];
		std::string email = x["email"];

		if (storage.AddUser(username, password, email))
			return crow::response{ "true" };

		return crow::response{ "false" };
			});

	CROW_ROUTE(m_app, "/loginUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];

		if (!storage.CheckUsername(username))
			return crow::response{ 409, "Username does not exist!" };

		if (!storage.CheckPassword(username, password))
			return crow::response{ 409, "Wrong password!" };

		return crow::response{ 200 };

			});

	CROW_ROUTE(m_app, "/roomID")
		.methods("GET"_method, "POST"_method)([&]() {
		std::string roomID = storage.GetRandomID();
		
		while (storage.CheckRoomID(roomID) && storage.GetGame(roomID).getGameStatusAsInt() != 3) 
			roomID = storage.GetRandomID();
		
		return crow::response{ roomID };
			});


	CROW_ROUTE(m_app, "/createRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["gameCode"];
		std::string username = x["username"];
		int maxPlayers = std::stoi(x["maxPlayers"]);
		int currentPlayers = std::stoi(x["currentPlayers"]);
		Player player = storage.GetPlayer(username);
		if(!storage.AddGame(player,roomID,maxPlayers))
			return crow::response{409, "Error creating the game."};

		return crow::response{ 200 };
			});

	CROW_ROUTE(m_app, "/checkRoomID")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (!storage.CheckRoomID(roomID))
			return crow::response{ 409, "Game not found"};

		if(storage.GetGame(roomID).getCurrentPlayers()>= storage.GetGame(roomID).getMaxPlayers())
			return crow::response{ 409, "Game is full" };

		if(storage.GetGame(roomID).getGameStatusAsInt() != 1)
			return crow::response{ 409, "Game already started" };

		return crow::response{ 200 };
			});

	m_app.port(18080).multithreaded().run();
}
