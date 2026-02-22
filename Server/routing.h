#pragma once
#include <crow.h>
#include "database.h"
#include "../PasswordHashing/Hashing.h"

import game;

class Routing
{
public:
	Routing() = delete;
	Routing(const Database& storage);

	void Run();
private:
	[[nodiscard]] std::shared_ptr<Game> GetGameByRoomID(const std::string& roomID) noexcept;
	[[nodiscard]] std::shared_ptr<Player> GetPlayerByUsername(const std::string& username) noexcept;

private:
	Routing(const Routing&) = delete;
	Routing& operator=(const Routing&) = delete;

private:
	std::map<std::string, std::shared_ptr<Game>> m_games; // Map to store the games based on the game code

	std::map<std::string, std::shared_ptr<Player>> m_players; // Map to store the players based on the username

private:
	Database m_storage;
	crow::SimpleApp m_app;
};
