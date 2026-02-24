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
