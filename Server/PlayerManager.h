#pragma once
#include <memory>
#include <map>
#include <shared_mutex>
#include <string>
#include <crow.h>
#include "database.h"

import player;

class PlayerManager
{
public:
	explicit PlayerManager(Database& storage);

	// Player operations
	[[nodiscard]] std::shared_ptr<Player> GetPlayerByUsername(const std::string& username) noexcept;
	void AddPlayer(const std::string& username, std::shared_ptr<Player> player);

	// Route handlers
	crow::response HandleCheckUsername(const crow::request& req);
	crow::response HandleAddUser(const crow::request& req);
	crow::response HandleLoginUser(const crow::request& req);

private:
	std::map<std::string, std::shared_ptr<Player>> m_players;
	mutable std::shared_mutex m_playersMutex;
	Database& m_storage;
};
