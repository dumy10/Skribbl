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
	StartCleanupTimer();
}

Routing::~Routing()
{
	StopCleanupTimer();
}

crow::response Routing::HandleRoot()
{
	return crow::response{ "Hello, Skribbl World!" };
}

crow::response Routing::HandleRandomWord()
{
	return crow::response{ 200, m_storage.GetRandomWord() };
}

crow::response Routing::HandleCheckUsername(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string username = params.at("username");

	if (username.empty()) {
		return crow::response{ 404 };
	}

	if (m_storage.CheckUsername(std::move(username))) {
		return crow::response{ 200, "EXISTING" };
	}

	return crow::response{ 404 };
}

crow::response Routing::HandleAddUser(const crow::request& req)
{
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

	std::unique_lock lock{ m_playersMutex };
	m_players.insert_or_assign(player.value()->GetName(), *player);

	return crow::response{ 201 };
}

crow::response Routing::HandleLoginUser(const crow::request& req)
{
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
}

crow::response Routing::HandleGetRoomID()
{
	std::string roomID = m_storage.GetRandomID();
	auto game = GetGameByRoomID(roomID);

	while (game != nullptr) {
		roomID = m_storage.GetRandomID();
		game = GetGameByRoomID(roomID);
	}

	return crow::response{ 201, roomID };
}

crow::response Routing::HandleCreateRoom(const crow::request& req)
{
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

	game.value()->GetRound() = Round{ std::move(roomID), {}, static_cast<size_t>(maxPlayers) };

	std::unique_lock lock{ m_gamesMutex };
	m_games.insert_or_assign(roomID, game.value());

	return crow::response{ 201 };
}

crow::response Routing::HandleCheckRoomID(const crow::request& req)
{
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
}

crow::response Routing::HandleJoinRoom(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");
	std::string username = params.at("username");

	auto player = GetPlayerByUsername(username);

	if (!player) {
		return crow::response{ 404 };
	}

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	game->AddPlayer(std::move(*player));

	return crow::response{ 200 };
}

crow::response Routing::HandleGetRoomPlayers(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ game->SerializePlayerNames() };
}

crow::response Routing::HandleGetNumberOfPlayers(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetNumberOfMaxPlayers()) };
}

crow::response Routing::HandleGetCurrentNumberOfPlayers(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetCurrentPlayerCount()) };
}

crow::response Routing::HandleLeaveRoom(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");
	std::string username = params.at("username");

	auto player = GetPlayerByUsername(username);

	if (!player) {
		return crow::response{ 404 };
	}

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	game->RemovePlayer(std::move(*player));

	if (game->GetCurrentPlayerCount() == 0) {
		std::unique_lock lock{ m_gamesMutex };
		m_games.erase(roomID);
		m_storage.Update(*game);
	}

	return crow::response{ 200 };
}

crow::response Routing::HandleGameStarted(const crow::request& req)
{
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
}

crow::response Routing::HandleStartGame(const crow::request& req)
{
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
}

crow::response Routing::HandleGetPlayerScore(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string username = params.at("username");
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetPlayerScore(std::move(username))) };
}

crow::response Routing::HandleAddChat(const crow::request& req)
{
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
}

crow::response Routing::HandleGetChat(const crow::request& req)
{
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
}

crow::response Routing::HandleGetDrawingPlayer(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ game->GetDrawingPlayer() };
}

crow::response Routing::HandleGetRoundNumber(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetRound().GetRoundNumber()) + "/" + std::to_string(game->GetMaxNumberOfRoundsAllowed()) };
}

crow::response Routing::HandleGetCurrentWord(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ game->GetRound().GetCurrentWord() };
}

crow::response Routing::HandleGetTimeLeft(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetRound().GetTimeLeft()) };
}

crow::response Routing::HandleNextRound(const crow::request& req)
{
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
}

crow::response Routing::HandleGameEnded(const crow::request& req)
{
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
}

crow::response Routing::HandleEndGame(const crow::request& req)
{
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
}

crow::response Routing::HandleDrawingImage(const crow::request& req)
{
	if (req.method == crow::HTTPMethod::Post) {
		const std::string& roomID = req.url_params.get("roomID");
		const std::string& imageData = req.body;

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		game->GetRound().SetImageData(imageData);

		return crow::response{ 200 };
	} else if (req.method == crow::HTTPMethod::Get) {
		auto params = parseUrlArgs(req.body);
		std::string roomID = params.at("roomID");

		auto game = GetGameByRoomID(roomID);

		if (!game) {
			return crow::response{ 404 };
		}

		return crow::response{ game->GetRound().GetImageData() };
	}

	return crow::response{ 400 };
}

crow::response Routing::HandleClearImage(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	game->GetRound().SetImageData("");

	return crow::response{ 200 };
}

crow::response Routing::HandleAllPlayersGuessed(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	if (game->ShouldEndGame()) {
		if (!game->EndGame()) {
			return crow::response{ 400, "Error ending the game." };
		}
		m_storage.Update(*game);
		return crow::response{ 200, "GAME_ENDED" };
	}

	if (game->GetAllPlayersGuessedWord()) {
		return crow::response{ 200, "TRUE" };
	}

	return crow::response{ 200, "FALSE" };
}

void Routing::Run()
{
	// Test route
	CROW_ROUTE(m_app, "/")([this]() { return HandleRoot(); });

	// Word management
	CROW_ROUTE(m_app, "/randomWord")([this]() { return HandleRandomWord(); });

	// User authentication routes
	CROW_ROUTE(m_app, "/checkUsername").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleCheckUsername(req); });
	CROW_ROUTE(m_app, "/addUser").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleAddUser(req); });
	CROW_ROUTE(m_app, "/loginUser").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleLoginUser(req); });

	// Room management routes
	CROW_ROUTE(m_app, "/roomID").methods(crow::HTTPMethod::Get)([this]() { return HandleGetRoomID(); });
	CROW_ROUTE(m_app, "/createRoom").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleCreateRoom(req); });
	CROW_ROUTE(m_app, "/checkRoomID").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleCheckRoomID(req); });
	CROW_ROUTE(m_app, "/joinRoom").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleJoinRoom(req); });
	CROW_ROUTE(m_app, "/leaveRoom").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleLeaveRoom(req); });
	CROW_ROUTE(m_app, "/roomPlayers").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetRoomPlayers(req); });
	CROW_ROUTE(m_app, "/numberOfPlayers").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetNumberOfPlayers(req); });
	CROW_ROUTE(m_app, "/currentNumberOfPlayers").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetCurrentNumberOfPlayers(req); });

	// Game state routes
	CROW_ROUTE(m_app, "/gameStarted").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGameStarted(req); });
	CROW_ROUTE(m_app, "/startGame").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleStartGame(req); });
	CROW_ROUTE(m_app, "/gameEnded").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGameEnded(req); });
	CROW_ROUTE(m_app, "/endGame").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleEndGame(req); });

	// Game play routes
	CROW_ROUTE(m_app, "/playerScore").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetPlayerScore(req); });
	CROW_ROUTE(m_app, "/drawingPlayer").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetDrawingPlayer(req); });
	CROW_ROUTE(m_app, "/roundNumber").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetRoundNumber(req); });
	CROW_ROUTE(m_app, "/currentWord").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetCurrentWord(req); });
	CROW_ROUTE(m_app, "/timeLeft").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetTimeLeft(req); });
	CROW_ROUTE(m_app, "/nextRound").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleNextRound(req); });
	CROW_ROUTE(m_app, "/allPlayersGuessed").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleAllPlayersGuessed(req); });

	// Chat routes
	CROW_ROUTE(m_app, "/addChat").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleAddChat(req); });
	CROW_ROUTE(m_app, "/getChat").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return HandleGetChat(req); });

	// Drawing routes
	CROW_ROUTE(m_app, "/drawingImage").methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleDrawingImage(req); });
	CROW_ROUTE(m_app, "/clearImage").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return HandleClearImage(req); });

	m_app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}

std::shared_ptr<Game> Routing::GetGameByRoomID(const std::string& roomID) noexcept
{
	std::shared_lock lock{ m_gamesMutex };

	auto it = m_games.find(roomID);
	if (it != m_games.end()) {
		return it->second;
	}

	return m_storage.GetGame(roomID).value_or(nullptr);
}

std::shared_ptr<Player> Routing::GetPlayerByUsername(const std::string& username) noexcept
{
	std::shared_lock lock{ m_playersMutex };

	auto it = m_players.find(username);
	if (it != m_players.end()) {
		return it->second;
	}

	return m_storage.GetPlayer(username).value_or(nullptr);
}

void Routing::CleanupFinishedGames()
{
	std::unique_lock lock{ m_gamesMutex };

	auto now = std::chrono::steady_clock::now();

	auto shouldRemove = [&now, &storage = m_storage](const auto& pair) {
		const auto& [roomID, game] = pair;

		if (game->GetGameStatus() == GameStatus::FINISHED) {
			return true;
		}

		auto timeSinceLastActivity = std::chrono::duration_cast<std::chrono::minutes>(
			now - game->GetLastActivityTime()
		);

		if (timeSinceLastActivity < kGameInactivityThreshold) {
			return false;
		}

		if (game->EndGame()) {
			storage.Update(*game);
		}
		
		return true;
		};

	auto toRemove = m_games
		| std::views::filter(shouldRemove)
		| std::views::keys;

	for (const auto& roomID : toRemove | std::ranges::to<std::vector>()) {
		m_games.erase(roomID);
	}
}

void Routing::StartCleanupTimer()
{
	m_isRunning = true;
	m_cleanupThread = std::thread([this]() {
		while (m_isRunning) {
			std::this_thread::sleep_for(kCleanupInterval);
			if (m_isRunning) { // Check again after waking up to avoid race condition
				CleanupFinishedGames();
			}
		}
		});
}

void Routing::StopCleanupTimer()
{
	m_isRunning = false;
	if (m_cleanupThread.joinable()) {
		m_cleanupThread.join();
	}
}
