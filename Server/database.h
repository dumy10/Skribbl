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
		),
		sql::make_table(
			"Rounds",
			sql::make_column("id", &Round::SetId, &Round::GetId, sql::primary_key().autoincrement()),
			sql::make_column("gameid", &Round::SetGameId, &Round::GetGameId),
			sql::make_column("word", &Round::SetCurrentWord, &Round::GetCurrentWord),
			sql::make_column("drawingplayer", &Round::SetDrawingPlayer, &Round::GetDrawingPlayer),
			sql::make_column("roundnumber", &Round::SetRoundNumber, &Round::GetRoundNumber),
			sql::make_column("words", &Round::SerializeWords, &Round::DeserializeWords),
			sql::make_column("timeleft", &Round::SetTimeLeft, &Round::GetTimeLeft),
			sql::make_column("times", &Round::SerializeTimes, &Round::DeserializeTimes),
			sql::make_column("imagedata", &Round::SetImageData, &Round::GetImageData)
		)
	);
}

using Storage = decltype(CreateStorage(""));

class Database
{
public:
	
	bool Initialize(); // Initializes the database with the words from the file

	std::string GetRandomWord(); // Gets a random word from the database

	bool AddUser(const std::string& username, const std::string& password, const std::string& email); // Adds a player to the database
	bool AddGame(const Player& player, const std::string& gameCode, size_t maxPlayers); // Adds a game to the database
	bool SetGameStatus(const std::string& roomID, int status); // Sets the game status in the database based on the roomID
	bool SetGameChat(const std::string& roomID, const std::string& chat); // Sets the game chat in the database based on the roomID
	bool AddPlayerToGame(const Player& player, const std::string& roomID, int currentPlayers); // Adds a player to an existing game based on the roomID in the database and updates the currentPlayers count
	bool RemovePlayerFromGame(const Player& player, const std::string& roomID); // Removes a player from an existing game based on the roomID in the database and updates the currentPlayers count
	bool CheckUsername(const std::string& username); //Checks if a username exists in the database
	bool CheckRoomID(const std::string& roomID); // Checks if a game with the roomID exists in the database
	bool CheckPassword(const std::string& username, const std::string& password); //Checks if a username and password match

	Game GetGame(const std::string& roomID); // Gets a game from the database based on the roomID
	Round GetRound(const std::string& roomID); // Gets a round from the database based on the roomID
	Player GetPlayer(const std::string& username); // Gets a player from the database based on the username
	int GetPlayerScore(const std::string& username); // Gets the players score from the database based on the username
	void SetPlayerScore(const std::string& username, int score); // Sets the players score in the database based on the username

	std::string GetRandomID(); //Gets a random ID for game

	template<typename T>
	void Update(T object); // Updates an object in the database

private:
	void PopulateStorage();
	int GenerateRandomNumber(int min, int max);
private:
	const std::string kDbFile{ "skribbl.sqlite" };
private:
	Storage m_db = CreateStorage(kDbFile);
};

template<typename T>
inline void Database::Update(T object)
{
	m_db.update(object);
}
