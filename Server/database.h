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

using namespace skribbl;

/*
Todo:
A table with the words should be created that should be initiallized with the words from the file

*/


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
		)
	);
}

using Storage = decltype(CreateStorage(""));

/*
TODO:
- Add a player to the database (register)
- Check if a player exists in the database
- Get a player from the database
- Allow the player to login (check if user exists and if the password is correct)
*/

class Database
{
public:
	// Initializes the database with the words from the file
	bool Initialize();
	// Gets a random word from the database
	std::string GetRandomWord();
	std::vector<Word> GetWords();

	// Adds a player to the database
	bool AddPlayer(const Player& player);

	// Checks if the player exists in the database
	bool PlayerExists(const Player& player);

	// Gets the player from the database
	Player GetPlayer(const Player& player);

private:
	void PopulateStorage();
	int GenerateRandomNumber(int min, int max);
private:
	const std::string kDbFile{ "skribbl.sqlite" };
private:
	Storage m_db = CreateStorage(kDbFile);
};