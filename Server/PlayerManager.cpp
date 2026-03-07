#include "PlayerManager.h"
#include "../PasswordHashing/Hashing.h"

import player;
import utils;

using namespace skribbl;

PlayerManager::PlayerManager(Database& storage)
	: m_storage{ storage }
{
	std::vector<std::shared_ptr<Player>> players = m_storage.GetAllPlayers();
	for (const auto& player : players) {
		m_players.insert_or_assign(player->GetName(), player);
	}
}

std::shared_ptr<Player> PlayerManager::GetPlayerByUsername(const std::string& username) noexcept
{
	std::shared_lock lock{ m_playersMutex };

	auto it = m_players.find(username);
	if (it != m_players.end()) {
		return it->second;
	}

	return m_storage.GetPlayer(username).value_or(nullptr);
}

void PlayerManager::AddPlayer(const std::string& username, std::shared_ptr<Player> player)
{
	std::unique_lock lock{ m_playersMutex };
	m_players.insert_or_assign(username, std::move(player));
}

bool PlayerManager::IsPlayerOnline(const std::string& username) const noexcept
{
	std::shared_lock lock{ m_onlinePlayersMutex };
	return m_onlinePlayers.find(username) != m_onlinePlayers.end();
}

void PlayerManager::MarkPlayerOnline(const std::string& username)
{
	std::unique_lock lock{ m_onlinePlayersMutex };
	m_onlinePlayers.insert(username);
}

void PlayerManager::MarkPlayerOffline(const std::string& username)
{
	std::unique_lock lock{ m_onlinePlayersMutex };
	m_onlinePlayers.erase(username);
}

crow::response PlayerManager::HandleCheckUsername(const crow::request& req)
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

crow::response PlayerManager::HandleAddUser(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string username = params.at("username");
	std::string password = params.at("password");
	std::string salt = params.at("salt");
	std::string email = params.at("email");
	
	if (username.empty() || password.empty() || salt.empty() || email.empty()) {
		return crow::response{ 404 };
	}
	
	auto player = m_storage.AddUser(std::move(username), std::move(password), std::move(salt), std::move(email));

	if (!player.has_value()) {
		return crow::response{ 400 };
	}

	AddPlayer(player.value()->GetName(), *player);

	return crow::response{ 201 };
}

crow::response PlayerManager::HandleLoginUser(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string username = params.at("username");
	std::string password = params.at("password");

	if (username.empty() || password.empty()) {
		return crow::response{ 404 };
	}

	if (!m_storage.CheckUsername(username)) {
		return crow::response{ 404 };
	}

	if (!m_storage.CheckPassword(username, password)) {
		return crow::response{ 404 };
	}

	// Check if player is already logged in
	if (IsPlayerOnline(username)) {
		return crow::response{ 409, "ALREADY_LOGGED_IN" }; // 409 Conflict
	}

	MarkPlayerOnline(username);

	return crow::response{ 204 };
}

crow::response PlayerManager::HandleLogoutUser(const crow::request& req)
{
	auto params = parseUrlArgs(req.body);
	std::string username = params.at("username");

	if (username.empty()) {
		return crow::response{ 404 };
	}

	MarkPlayerOffline(username);

	return crow::response{ 204 };
}
