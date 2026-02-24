#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

import player;
import word;
import game;

using namespace skribbl;

inline auto CreateStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table(
			"Players",
			sql::make_column("id", &Player::SetId, &Player::GetId, sql::primary_key().autoincrement()),
			sql::make_column("name", &Player::SetName, &Player::GetName),
			sql::make_column("email", &Player::SetEmail, &Player::GetEmail),
			sql::make_column("password", &Player::SetPassword, &Player::GetPassword),
			sql::make_column("score", &Player::SetPoints, &Player::GetPoints)
		),
		sql::make_table(
			"Words",
			sql::make_column("id", &Word::SetId, &Word::GetId, sql::primary_key().autoincrement()),
			sql::make_column("word", &Word::SetWord, &Word::GetWord)
		),
		sql::make_table(
			"Games",
			sql::make_column("id", &Game::SetId, &Game::GetId, sql::primary_key().autoincrement()),
			sql::make_column("players", &Game::SerializePlayers, &Game::DeserializePlayers),
			sql::make_column("gamecode", &Game::SetGameCode, &Game::GetGameCode),
			sql::make_column("maxplayers", &Game::SetMaxPlayers, &Game::GetNumberOfMaxPlayers),
			sql::make_column("status", &Game::SetGameStatusFromInt, &Game::GetGameStatusAsInt),
			sql::make_column("chat", &Game::SerializeGameChat, &Game::DeserializeGameChat)
		)
	);
}

using Storage = decltype(CreateStorage(""));

class Database
{
public:
	
	[[nodiscard]] bool Initialize(); // Initializes the database with the words from the file

	[[nodiscard]] std::string GetRandomWord(); // Gets a random word from the database

	[[nodiscard]] std::optional<std::shared_ptr<Player>> AddUser(const std::string& username, const std::string& password, const std::string& email); // Adds a player to the database
	[[nodiscard]] std::optional<std::shared_ptr<Game>> AddGame(const Player& player, const std::string& gameCode, size_t maxPlayers); // Adds a game to the database
	[[nodiscard]] bool CheckUsername(const std::string& username); //Checks if a username exists in the database
	[[nodiscard]] bool CheckRoomID(const std::string& roomID); // Checks if a game with the roomID exists in the database
	[[nodiscard]] bool CheckPassword(const std::string& username, const std::string& password); //Checks if a username and password match

	[[nodiscard]] std::optional<std::shared_ptr<Game>> GetGame(const std::string& roomID); // Gets a game from the database based on the roomID
	[[nodiscard]] std::optional<std::shared_ptr<Player>> GetPlayer(const std::string& username); // Gets a player from the database based on the username
	[[nodiscard]] std::vector<std::shared_ptr<Player>> GetAllPlayers(); // Gets all players from the database

	[[nodiscard]] std::string GetRandomID(); //Gets a random ID for game

	template<typename T>
	void Update(const T& object); // Updates an object in the database

private:
	void PopulateStorage();
	int GenerateRandomNumber(int min, int max);
private:
	const std::string kDbFile{ "skribbl.sqlite" };
private:
	Storage m_db = CreateStorage(kDbFile);
};

template<typename T>
inline void Database::Update(const T& object)
{
	m_db.update(object);
}
