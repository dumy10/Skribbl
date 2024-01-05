#pragma once
#include <string>
#include <vector>
#include <random>
#include <fstream>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

import utils;
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
			sql::make_column("password", &Player::SetPassword, &Player::GetPassword),
			sql::make_column("email", &Player::SetEmail, &Player::GetEmail),
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
			sql::make_column("maxplayers", &Game::SetMaxPlayers, &Game::GetMaxPlayers),
			sql::make_column("currentplayers", &Game::SetCurrentPlayers, &Game::GetCurrentPlayers),
			sql::make_column("status", &Game::SetGameStatusInt, &Game::GetGameStatusAsInt),
			sql::make_column("chat", &Game::SetChat, &Game::GetChat)
		)

	);
}

using Storage = decltype(CreateStorage(""));

class Database
{
public:
	// Initializes the database with the words from the file
	bool Initialize();

	// Gets a random word from the database
	std::string GetRandomWord();
	std::vector<Word> GetWords();
	std::vector<std::string> GetCustomNumberOfWords(int numberOfWords);

	//Gets all the stored games in the database *debugging purposes, will be removed later*
	std::vector<Game> GetGames();
	std::vector<Player> GetPlayers();

	// Adds a player to the database
	bool AddUser(const std::string& username, const std::string& password, const std::string& email);

	// Adds a game to the database
	bool AddGame(const Player& player, const std::string& gameCode, size_t maxPlayers);
	bool SetGameStatus(const std::string& roomID, int status);


	// Gets a game from the database based on the roomID
	Game GetGame(const std::string& roomID);

	// Sets the game chat in the database based on the roomID
	bool SetGameChat(const std::string& roomID, const std::string& chat);

	// Gets the players score from the database based on the username
	int GetPlayerScore(const std::string& username);
	
	// Sets the players score in the database based on the username
	void SetPlayerScore(const std::string& username, int score);

	// Adds a player to an existing game based on the roomID in the database and updates the currentPlayers count
	bool AddPlayerToGame(const Player& player, const std::string& roomID, int currentPlayers);

	// Removes a player from an existing game based on the roomID in the database and updates the currentPlayers count
	bool RemovePlayerFromGame(const Player& player, const std::string& roomID);

	// Gets a player from the database based on the username
	Player GetPlayer(const std::string& username);

	//Checks if a username exists in the database
	bool CheckUsername(const std::string& username);

	// Checks if a game with the roomID exists in the database
	bool CheckRoomID(const std::string& roomID);

	//Checks if a username and password match
	bool CheckPassword(const std::string& username, const std::string& password);

	//Gets a random ID for game
	std::string GetRandomID();

private:
	void PopulateStorage();
	int GenerateRandomNumber(int min, int max);
private:
	const std::string kDbFile{ "skribbl.sqlite" };
private:
	Storage m_db = CreateStorage(kDbFile);
};