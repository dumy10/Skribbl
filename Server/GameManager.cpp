#include "GameManager.h"
#include "PlayerManager.h"

import player;
import game;
import round;
import utils;

using namespace skribbl;

GameManager::GameManager(Database& storage)
	: m_storage{ storage }
{
	StartCleanupTimer();
}

GameManager::~GameManager()
{
	StopCleanupTimer();
}

std::shared_ptr<Game> GameManager::GetGameByRoomID(const std::string& roomID) noexcept
{
	std::shared_lock lock{ m_gamesMutex };

	auto it = m_games.find(roomID);
	if (it != m_games.end()) {
		return it->second;
	}

	return m_storage.GetGame(roomID).value_or(nullptr);
}

void GameManager::AddGame(const std::string& roomID, std::shared_ptr<Game> game)
{
	std::unique_lock lock{ m_gamesMutex };
	m_games.insert_or_assign(roomID, std::move(game));
}

void GameManager::RemoveGame(const std::string& roomID)
{
	std::unique_lock lock{ m_gamesMutex };
	m_games.erase(roomID);
}

crow::response GameManager::HandleGetRoomID()
{
	std::string roomID = m_storage.GetRandomID();
	auto game = GetGameByRoomID(roomID);

	while (game != nullptr) {
		roomID = m_storage.GetRandomID();
		game = GetGameByRoomID(roomID);
	}

	return crow::response{ 201, roomID };
}

crow::response GameManager::HandleCreateRoom(const crow::request& req, PlayerManager& playerManager)
{
	auto params = parseUrlArgs(req.body);

	std::string roomID = params.at("roomID");
	std::string username = params.at("username");
	int maxPlayers = std::stoi(params.at("maxPlayers"));
	int currentPlayers = std::stoi(params.at("currentPlayers"));

	auto player = playerManager.GetPlayerByUsername(username);

	if (!player) {
		return crow::response{ 404 };
	}

	auto game = m_storage.AddGame(std::move(*player), std::move(roomID), maxPlayers);
	if (!game.has_value()) {
		return crow::response{ 400, "Error creating the game." };
	}

	game.value()->GetRound() = Round{ std::move(roomID), {}, static_cast<size_t>(maxPlayers) };

	AddGame(roomID, game.value());

	return crow::response{ 201 };
}

crow::response GameManager::HandleCheckRoomID(const crow::request& req)
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

crow::response GameManager::HandleJoinRoom(const crow::request& req, PlayerManager& playerManager)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");
	std::string username = params.at("username");

	auto player = playerManager.GetPlayerByUsername(username);

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

crow::response GameManager::HandleGetRoomPlayers(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ game->SerializePlayerNames() };
}

crow::response GameManager::HandleGetNumberOfPlayers(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetNumberOfMaxPlayers()) };
}

crow::response GameManager::HandleGetCurrentNumberOfPlayers(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetCurrentPlayerCount()) };
}

crow::response GameManager::HandleLeaveRoom(const crow::request& req, PlayerManager& playerManager)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");
	std::string username = params.at("username");

	auto player = playerManager.GetPlayerByUsername(username);

	if (!player) {
		return crow::response{ 404 };
	}

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	game->RemovePlayer(std::move(*player));

	if (game->GetCurrentPlayerCount() == 0) {
		RemoveGame(roomID);
		m_storage.Update(*game);
	}

	return crow::response{ 200 };
}

crow::response GameManager::HandleGameStarted(const crow::request& req)
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

crow::response GameManager::HandleStartGame(const crow::request& req)
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

crow::response GameManager::HandleGetPlayerScore(const crow::request& req)
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

crow::response GameManager::HandleAddChat(const crow::request& req)
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

crow::response GameManager::HandleGetChat(const crow::request& req)
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

crow::response GameManager::HandleGetDrawingPlayer(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ game->GetDrawingPlayer() };
}

crow::response GameManager::HandleGetRoundNumber(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetRound().GetRoundNumber()) + "/" + std::to_string(game->GetMaxNumberOfRoundsAllowed()) };
}

crow::response GameManager::HandleGetCurrentWord(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ game->GetRound().GetCurrentWord() };
}

crow::response GameManager::HandleGetTimeLeft(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string roomID = params.at("roomID");

	auto game = GetGameByRoomID(roomID);

	if (!game) {
		return crow::response{ 404 };
	}

	return crow::response{ std::to_string(game->GetRound().GetTimeLeft()) };
}

crow::response GameManager::HandleNextRound(const crow::request& req)
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

crow::response GameManager::HandleGameEnded(const crow::request& req)
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

crow::response GameManager::HandleEndGame(const crow::request& req)
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

crow::response GameManager::HandleDrawingImage(const crow::request& req)
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

crow::response GameManager::HandleClearImage(const crow::request& req)
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

crow::response GameManager::HandleAllPlayersGuessed(const crow::request& req)
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

void GameManager::CleanupFinishedGames()
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

void GameManager::StartCleanupTimer()
{
	m_isRunning = true;
	m_cleanupThread = std::thread([this]() {
		while (m_isRunning) {
			std::this_thread::sleep_for(kCleanupInterval);
			if (m_isRunning) {
				CleanupFinishedGames();
			}
		}
	});
}

void GameManager::StopCleanupTimer()
{
	m_isRunning = false;
	if (m_cleanupThread.joinable()) {
		m_cleanupThread.join();
	}
}
