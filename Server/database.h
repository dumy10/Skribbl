#pragma once
#include <string>
#include <vector>
#include <random>

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
			sql::make_column("id", &Player::setId, &Player::getId, sql::primary_key().autoincrement()),
			sql::make_column("name", &Player::setName, &Player::getName),
			sql::make_column("password", &Player::setPassword, &Player::getPassword),
			sql::make_column("email", &Player::setEmail, &Player::getEmail),
			sql::make_column("score", &Player::setPoints, &Player::getPoints)
		),
		sql::make_table(
			"Words",
			sql::make_column("id", &Word::setId, &Word::getId, sql::primary_key().autoincrement()),
			sql::make_column("word", &Word::setWord, &Word::getWord)
		),
		sql::make_table(
			"Games",
			sql::make_column("id", &Game::setId, &Game::getId, sql::primary_key().autoincrement()),
			sql::make_column("players", &Game::serializePlayers, &Game::deserializePlayers),
			sql::make_column("gamecode", &Game::setGameCode, &Game::getGameCode),
			sql::make_column("maxplayers", &Game::setMaxPlayers, &Game::getMaxPlayers),
			sql::make_column("currentplayers", &Game::setCurrentPlayers, &Game::getCurrentPlayers),
			sql::make_column("status", &Game::setGameStatusInt, &Game::getGameStatusAsInt)
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

	// Adds a player to the database
	bool AddUser(const std::string& username, const std::string& password, const std::string& email);

	bool AddGame(const Player& player, const std::string& gameCode, size_t maxPlayers);

	Game GetGame(const std::string& roomID);

	Player GetPlayer(const std::string& username);

	//Checks if a username exists in the database
	bool CheckUsername(const std::string& username);

	bool CheckRoomID(const std::string& roomID);

	//Checks if a username and password match
	bool CheckPassword(const std::string& username, const std::string& password);
	//Gets a random ID for game
	std::string GetRandomID() ;

private:
	void PopulateStorage();
	int GenerateRandomNumber(int min, int max) ;
private:
	const std::string kDbFile{ "skribbl.sqlite" };
private:
	Storage m_db = CreateStorage(kDbFile);
};