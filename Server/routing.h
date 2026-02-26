#pragma once
#include <crow.h>
#include <shared_mutex>
#include "database.h"
#include "../PasswordHashing/Hashing.h"

import game;

class Routing
{
public:
	Routing() = delete;
	Routing(const Database& storage);
	~Routing();

	void Run();

private:
	// Route Handlers
	crow::response HandleRoot();
	crow::response HandleRandomWord();
	crow::response HandleCheckUsername(const crow::request& req);
	crow::response HandleAddUser(const crow::request& req);
	crow::response HandleLoginUser(const crow::request& req);
	crow::response HandleGetRoomID();
	crow::response HandleCreateRoom(const crow::request& req);
	crow::response HandleCheckRoomID(const crow::request& req);
	crow::response HandleJoinRoom(const crow::request& req);
	crow::response HandleGetRoomPlayers(const crow::request& req);
	crow::response HandleGetNumberOfPlayers(const crow::request& req);
	crow::response HandleGetCurrentNumberOfPlayers(const crow::request& req);
	crow::response HandleLeaveRoom(const crow::request& req);
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

	// Helper Methods
	[[nodiscard]] std::shared_ptr<Game> GetGameByRoomID(const std::string& roomID) noexcept;
	[[nodiscard]] std::shared_ptr<Player> GetPlayerByUsername(const std::string& username) noexcept;
	void CleanupFinishedGames();
	void StartCleanupTimer();
	void StopCleanupTimer();

private:
	Routing(const Routing&) = delete;
	Routing& operator=(const Routing&) = delete;

private:
	std::map<std::string, std::shared_ptr<Game>> m_games; // Map to store the games based on the game code
	std::map<std::string, std::shared_ptr<Player>> m_players; // Map to store the players based on the username

private:
	mutable std::shared_mutex m_gamesMutex;
	mutable std::shared_mutex m_playersMutex;

	std::thread m_cleanupThread; 
	std::atomic<bool> m_isRunning{ false };

private:
	Database m_storage;
	crow::SimpleApp m_app;

	static constexpr std::chrono::minutes kCleanupInterval{ 30 }; 
	static constexpr std::chrono::minutes kGameInactivityThreshold { 60 }; 
};
