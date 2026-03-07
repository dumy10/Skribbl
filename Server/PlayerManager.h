#pragma once
#include <memory>
#include <map>
#include <set>
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

	// Session management
	[[nodiscard]] bool IsPlayerOnline(const std::string& username) const noexcept;
	void MarkPlayerOnline(const std::string& username);
	void MarkPlayerOffline(const std::string& username);

	// Route handlers
	crow::response HandleCheckUsername(const crow::request& req);
	crow::response HandleAddUser(const crow::request& req);
	crow::response HandleLoginUser(const crow::request& req);
	crow::response HandleLogoutUser(const crow::request& req);

private:
	std::map<std::string, std::shared_ptr<Player>> m_players;
	std::set<std::string> m_onlinePlayers;
	mutable std::shared_mutex m_playersMutex;
	mutable std::shared_mutex m_onlinePlayersMutex;
	Database& m_storage;
};
