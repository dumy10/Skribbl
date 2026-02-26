#pragma once
#include <memory>
#include <map>
#include <shared_mutex>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <set>
#include <crow.h>
#include "database.h"

import game;

class GameManager
{
public:
	explicit GameManager(Database& storage);
	~GameManager();

	// Game operations
	[[nodiscard]] std::shared_ptr<Game> GetGameByRoomID(const std::string& roomID) noexcept;
	void AddGame(const std::string& roomID, std::shared_ptr<Game> game);
	void RemoveGame(const std::string& roomID);
	
	// Route handlers
	crow::response HandleGetRoomID();
	crow::response HandleCreateRoom(const crow::request& req, class PlayerManager& playerManager);
	crow::response HandleCheckRoomID(const crow::request& req);
	crow::response HandleJoinRoom(const crow::request& req, PlayerManager& playerManager);
	crow::response HandleGetRoomPlayers(const crow::request& req);
	crow::response HandleGetNumberOfPlayers(const crow::request& req);
	crow::response HandleGetCurrentNumberOfPlayers(const crow::request& req);
	crow::response HandleLeaveRoom(const crow::request& req, PlayerManager& playerManager);
	crow::response HandleGameStarted(const crow::request& req);
	crow::response HandleStartGame(const crow::request& req);
	crow::response HandleGetPlayerScore(const crow::request& req);
	crow::response HandleAddChat(const crow::request& req);
	crow::response HandleGetChat(const crow::request& req);
	crow::response HandleGetDrawingPlayer(const crow::request& req);
	crow::response HandleGetRoundNumber(const crow::request& req);
	crow::response HandleGetCurrentWord(const crow::request& req);
	crow::response HandleGetTimeLeft(const crow::request& req);
	crow::response HandleNextRound(const crow::request& req);
	crow::response HandleGameEnded(const crow::request& req);
	crow::response HandleEndGame(const crow::request& req);
	crow::response HandleDrawingImage(const crow::request& req);
	crow::response HandleClearImage(const crow::request& req);
	crow::response HandleAllPlayersGuessed(const crow::request& req);

private:
	void CleanupFinishedGames();
	void StartCleanupTimer();
	void StopCleanupTimer();

private:
	std::map<std::string, std::shared_ptr<Game>> m_games;
	mutable std::shared_mutex m_gamesMutex;
	
	std::thread m_cleanupThread;
	std::atomic<bool> m_isRunning{ false };
	
	Database& m_storage;
	
	static constexpr std::chrono::minutes kCleanupInterval{ 30 };
	static constexpr std::chrono::minutes kGameInactivityThreshold{ 60 };
};
