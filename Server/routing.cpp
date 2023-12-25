#include "routing.h"
import player;
import game;
using namespace skribbl;


void Routing::Run(Database& storage)
{
	// Just a test route
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

		if (username == "" || password == "" || email == "")
			return crow::response{ 404, "Username, password or email not found." };

		if (storage.AddUser(username, password, email))
			return crow::response{ "true" };

		return crow::response{ "false" };
			});

	CROW_ROUTE(m_app, "/loginUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];

		if (username == "" || password == "")
			return crow::response{ 404, "Username or password not found." };

		if (!storage.CheckUsername(username))
			return crow::response{ 409, "Username does not exist!" };

		if (!storage.CheckPassword(username, password))
			return crow::response{ 409, "Wrong password!" };

		return crow::response{ 200 };

			});

	CROW_ROUTE(m_app, "/roomID")
		.methods("GET"_method, "POST"_method)([&]() {
		std::string roomID = storage.GetRandomID();

		while (storage.CheckRoomID(roomID) && storage.GetGame(roomID).GetGameStatusAsInt() != 3)
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

		if (!storage.AddGame(player, roomID, maxPlayers))
			return crow::response{ 409, "Error creating the game." };

		return crow::response{ 200 };
			});

	CROW_ROUTE(m_app, "/checkRoomID")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (!storage.CheckRoomID(roomID))
			return crow::response{ 409, "Game not found" };

		if (storage.GetGame(roomID).GetGameStatusAsInt() != 1)
			return crow::response{ 409, "Game already started" };

		if (storage.GetGame(roomID).GetCurrentPlayers() >= storage.GetGame(roomID).GetMaxPlayers())
			return crow::response{ 409, "Game is full" };

		std::string currentPlayers = std::to_string(storage.GetGame(roomID).GetCurrentPlayers());
		return crow::response{ 200 , currentPlayers };
			});

	CROW_ROUTE(m_app, "/joinRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string username = x["username"];
		Player player = storage.GetPlayer(username);
		int currentPlayers = std::stoi(x["currentPlayers"]);

		if (!storage.AddPlayerToGame(player, roomID, currentPlayers))
			return crow::response{ 409, "Error joining the game." };

		return crow::response{ 200 };
			});

	CROW_ROUTE(m_app, "/roomPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::string players = storage.GetGame(roomID).SerializePlayers();

		return crow::response{ players };
			});

	CROW_ROUTE(m_app, "/numberOfPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::string numberOfPlayers = std::to_string(storage.GetGame(roomID).GetMaxPlayers());

		return crow::response{ numberOfPlayers };
			});

	CROW_ROUTE(m_app, "/currentNumberOfPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::string currentNumberOfPlayers = std::to_string(storage.GetGame(roomID).GetCurrentPlayers());

		return crow::response{ currentNumberOfPlayers };
			});


	CROW_ROUTE(m_app, "/leaveRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string username = x["username"];

		Player player = storage.GetPlayer(username);

		if (!storage.RemovePlayerFromGame(player, roomID))
			return crow::response{ 409, "Error leaving the game." };
		return crow::response{ 200 };
			});

	CROW_ROUTE(m_app, "/gameStarted")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (storage.GetGame(roomID).GetGameStatusAsInt() == 2)
			return crow::response{ 200 };

		return crow::response{ 409 };
			});

	CROW_ROUTE(m_app, "/startGame")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		/*
		This route should be modified later when StartGame() will be implemented.
		*/

		if (!storage.SetGameStatus(roomID, 2))
			return crow::response{ 409, "Error starting the game." };

		return crow::response{ 200 };
			});
	CROW_ROUTE(m_app,"/checkWord")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x=parseUrlArgs(req.body);
		std::string word=x["word"];
		std::string roomID=x["roomID"];
		std::string username=x["username"];
		Player player=storage.GetPlayer(username);
		//if (word == storage.GetGame(roomID).GetWord())
		//{
		//	//add points to the player
		//  player.AddPoints(100);
		//  return crow::response{ 200 };
		//}
		//else send the word to the other players

		return crow::response{ 202 };
			});

	CROW_ROUTE(m_app, "/gameEnded")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (storage.GetGame(roomID).GetGameStatusAsInt() == 3)
			return crow::response{ 200 };

		return crow::response{ 409 };
			});
	
	m_app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
