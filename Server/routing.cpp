#include "routing.h"

import player;
import game;
import utils;

using namespace skribbl;

Routing::Routing(const Database& storage)
	: m_storage{ storage }
{
	std::vector<std::shared_ptr<Player>> players = m_storage.GetAllPlayers();
	for (const auto& player : players) {
		m_players.insert_or_assign(player->GetName(), player);
	}
}

void Routing::Run()
{
	// Just a test route
	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });

	// Gets a random word from the database
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ 200, m_storage.GetRandomWord() };
		});

	// Checks if the username exists in the database
	CROW_ROUTE(m_app, "/checkUsername")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string username = params.at("username");

		if (username.empty()) {
			return crow::response{ 404 };
		}

		if (m_storage.CheckUsername(std::move(username))) {
			return crow::response{ 200, "EXISTING" };
		}

		return crow::response{ 404};
			});

	// Adds a user to the database
	CROW_ROUTE(m_app, "/addUser")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string username = params.at("username");
		std::string password = params.at("password");
		std::string email = params.at("email");

		if (username.empty() || password.empty() || email.empty()) {
			return crow::response{ 404 };
		}

		auto player = m_storage.AddUser(std::move(username), std::move(password), std::move(email));

		if (!player.has_value()) {
			return crow::response{ 400 };
		}

		m_players.insert_or_assign(player.value()->GetName(), *player);

		return crow::response{ 201 };

			});

	// Checks if the username and password match in the database
	CROW_ROUTE(m_app, "/loginUser")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string username = params.at("username");
		std::string password = params.at("password");

		std::string hashedPass = password;
		Hasher::HashPassword(hashedPass.c_str());
		std::string hashedPassStr{ hashedPass };

		if (username.empty() || password.empty()) {
			return crow::response{ 404 };
		}

		if (!m_storage.CheckUsername(std::move(username))) {
			return crow::response{ 404 };
		}

		if (!m_storage.CheckPassword(std::move(username), std::move(hashedPassStr))) {
			return crow::response{ 404 };
		}

		return crow::response{ 204 };
			});

	// Gets a random roomID from the database
	CROW_ROUTE(m_app, "/roomID")
		.methods(crow::HTTPMethod::Get)([&]() {
		std::string roomID = m_storage.GetRandomID();
		auto game = GetGameByRoomID(roomID);

		while (game != nullptr) {
			roomID = m_storage.GetRandomID();
			game = GetGameByRoomID(roomID);
		}

		return crow::response{ 201, roomID };
			});

	// Creates a game in the database
	CROW_ROUTE(m_app, "/createRoom")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);

		std::string roomID = params.at("roomID");
		std::string username = params.at("username");
		int maxPlayers = std::stoi(params.at("maxPlayers"));
		int currentPlayers = std::stoi(params.at("currentPlayers"));

		auto player = GetPlayerByUsername(username);

		if (!player) {
			return crow::response{ 404 };
		}

		auto game = m_storage.AddGame(std::move(*player), std::move(roomID), maxPlayers);
		if (!game.has_value()) {
			return crow::response{ 400, "Error creating the game." };
		}

		m_games.insert_or_assign(roomID, game.value());

		return crow::response{ 201 };
			});

	// Checks if the roomID exists in the database
	CROW_ROUTE(m_app, "/checkRoomID")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		if (game->GetGameStatus() != GameStatus::WAITING) {
			return crow::response{ 400, "Game already started" };
		}

		if (game->GetCurrentPlayerCount() >= game->GetNumberOfMaxPlayers()) {
			return crow::response{ 400, "Game is full" };
		}

		return crow::response{ 200 , std::to_string(game->GetCurrentPlayerCount()) };
			});

	// Adds a player to the game in the database
	CROW_ROUTE(m_app, "/joinRoom")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");
		std::string username = params.at("username");

		auto player = GetPlayerByUsername(username);

		if (!player) {
			return crow::response{ 404 };
		}

		int currentPlayers = std::stoi(params.at("currentPlayers"));

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		game->AddPlayer(std::move(*player));

		return crow::response{ 200 };
			});

	// Gets the name of the players from the game in the database
	CROW_ROUTE(m_app, "/roomPlayers")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ game->SerializePlayerNames() };
			});

	// Gets the max number of players from the game in the database
	CROW_ROUTE(m_app, "/numberOfPlayers")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ std::to_string(game->GetNumberOfMaxPlayers()) };
			});

	// Gets the current number of players from the game in the database
	CROW_ROUTE(m_app, "/currentNumberOfPlayers")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ std::to_string(game->GetCurrentPlayerCount()) };
			});

	// Removes a player from the game in the database
	CROW_ROUTE(m_app, "/leaveRoom")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");
		std::string username = params.at("username");

		auto player = m_storage.GetPlayer(std::move(username));

		if (!player.has_value()) {
			return crow::response{ 404 };
		}

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		game->RemovePlayer(std::move(*player.value()));

		return crow::response{ 200 };
			});

	// Checks if the game has started in the database
	CROW_ROUTE(m_app, "/gameStarted")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		if (game->GetGameStatus() == GameStatus::INPROGRESS) {
			return crow::response{ 200 };
		}

		return crow::response{ 400 };
			});

	// Starts the game based on the roomID in the database
	CROW_ROUTE(m_app, "/startGame")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto currentGame = GetGameByRoomID(roomID);

		if (!currentGame) {
			return crow::response{ 404 };
		}

		size_t numberOfWords = currentGame->GetPlayers().size() * currentGame->GetMaxNumberOfRoundsAllowed();

		std::set<std::string> words;
		while (words.size() < numberOfWords) {
			words.insert(m_storage.GetRandomWord());
		}

		if (!currentGame->StartGame(words)) {
			return crow::response{ 400, "Error starting the game." };
		}

		return crow::response{ 200 };
			});

	// Gets the player score from the database based on the username
	CROW_ROUTE(m_app, "/playerScore")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string username = params.at("username");
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ std::to_string(game->GetPlayerScore(std::move(username))) };
			});

	// Adds the text that the player wrote to the current chat in the database
	CROW_ROUTE(m_app, "/addChat")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");
		std::string username = params.at("username");
		std::string text = params.at("text");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		bool hasGuessedWord{ false };
		if (!game->AddChatLineMessage(text, username, hasGuessedWord)) {
			return crow::response{ 400, "Error adding the chat." };
		}

		if (hasGuessedWord) {
			return crow::response{ 200, "TRUE" };
		}

		return crow::response{ 200 };
			});

	// Gets the current chat from the database
	CROW_ROUTE(m_app, "/getChat")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		auto& chatLines = game->GetChatLines();

		std::string result;
		for (const auto& line : chatLines) {
			result += line + "\n";
		}
		return crow::response{ result };
			});

	// Gets the current drawing player name from the database
	CROW_ROUTE(m_app, "/drawingPlayer")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ game->GetDrawingPlayer() };
			});

	// Gets the current round number from the database
	CROW_ROUTE(m_app, "/roundNumber")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ std::to_string(game->GetRound().GetRoundNumber()) + "/" + std::to_string(game->GetMaxNumberOfRoundsAllowed()) };
			});

	// Gets the current word that needs to be drawn from the database
	CROW_ROUTE(m_app, "/currentWord")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ game->GetRound().GetCurrentWord() };
			});

	// Updates the time left from the owner of the game and gets the time left from the database
	CROW_ROUTE(m_app, "/timeLeft")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ std::to_string(game->GetRound().GetTimeLeft()) };
			});

	// Sets the next round in the database
	CROW_ROUTE(m_app, "/nextRound")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		if (!game->NextRound()) {
			return crow::response{ 400 };
		}

		return crow::response{ 200 };
			});

	// Checks if the game has ended in the database
	CROW_ROUTE(m_app, "/gameEnded")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		if (game->GetGameStatus() == GameStatus::FINISHED) {
			return crow::response{ 200 };
		}

		return crow::response{ 400 };
			});

	CROW_ROUTE(m_app, "/endGame")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params["roomID"];

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		if (!game->EndGame()) {
			return crow::response{ 400 };
		}

		m_storage.Update(*game);

		return crow::response{ 200 };
			});

	CROW_ROUTE(m_app, "/drawingImage")
		.methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post)([&](const crow::request& req) {

		if (req.method == crow::HTTPMethod::Post) {
			const std::string& roomID = req.url_params.get("roomID");
			const std::string& imageData = req.body;

			auto game = GetGameByRoomID(roomID);

			if (!game) {
				return crow::response{ 404 };
			}

			Round& currentRound = game->GetRound();
			currentRound.SetImageData(imageData);

			return crow::response{ 200 };
		}
		else if (req.method == crow::HTTPMethod::Get) {
			auto params = parseUrlArgs(req.body);
			std::string roomID = params.at("roomID");

			auto game = GetGameByRoomID(roomID);

			if (!game) {
				return crow::response{ 404 };
			}

			return crow::response{ game->GetRound().GetImageData() };
		}

		return crow::response{ 400 };
			});

	CROW_ROUTE(m_app, "/clearImage")
		.methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		Round& currentRound = game->GetRound();
		currentRound.SetImageData("");

		return crow::response{ 200 };
			});

	CROW_ROUTE(m_app, "/allPlayersGuessed")
		.methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		if (game->GetAllPlayersGuessedWord()) {
			return crow::response{ 200, "TRUE" };
		}

		return crow::response{ 200, "FALSE" };
			});

	m_app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}

std::shared_ptr<Game> Routing::GetGameByRoomID(const std::string& roomID) noexcept
{
	auto it = m_games.find(roomID);
	if (it != m_games.end()) {
		return it->second;
	}

	return m_storage.GetGame(roomID).value_or(nullptr);
}

std::shared_ptr<Player> Routing::GetPlayerByUsername(const std::string& username) noexcept
{
	auto it = m_players.find(username);
	if (it != m_players.end()) {
		return it->second;
	}

	return m_storage.GetPlayer(username).value_or(nullptr);
}
