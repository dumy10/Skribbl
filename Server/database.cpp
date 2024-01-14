#include "database.h"

bool Database::Initialize()
{
	m_db.sync_schema();
	auto initWordCount = m_db.count<Word>();

	if (initWordCount == 0)
		PopulateStorage();

	auto wordCount = m_db.count<Word>();
	return wordCount != 0;
}

std::string Database::GetRandomWord()
{
	auto wordCount = m_db.count<Word>();
	auto randomWordIndex = GenerateRandomNumber(1, wordCount);
	auto word = m_db.get<Word>(randomWordIndex);
	return word.GetWord();
}

bool Database::AddUser(const std::string& username, const std::string& password, const std::string& email)
{
	try
	{
		std::string correctEmail{ email };
		// if %40 is in the email, replace it with @
		if (correctEmail.find("%40") != std::string::npos)
			correctEmail.replace(correctEmail.find("%40"), 3, "@");
		m_db.insert(Player{ -1, username, password, correctEmail });
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding user: " << e.what() << "\n";
		return false;
	}
}

bool Database::AddGame(const Player& player, const std::string& gameCode, size_t maxPlayers)
{
	try
	{
		m_db.insert(Game{ -1, player, gameCode, maxPlayers, 1 });
		m_db.insert(Round{ -1, gameCode, maxPlayers });
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding game: " << e.what() << "\n";
		return false;
	}
}

bool Database::SetGameStatus(const std::string& roomID, int status)
{
	try
	{
		auto existingGames = m_db.get_all<Game>(
			sql::where(sql::c(&Game::GetGameCode) == roomID)
		);

		if (existingGames.empty())
			return false;

		auto& game = existingGames[0];
		game.SetGameStatusInt(status);
		m_db.update(game);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while setting game status: " << e.what() << "\n";
		return false;
	}
}

bool Database::SetGameChat(const std::string& roomID, const std::string& chat)
{
	try
	{
		auto existingGames = m_db.get_all<Game>(
			sql::where(sql::c(&Game::GetGameCode) == roomID)
		);

		if (existingGames.empty())
			return false;

		auto& game = existingGames[0];
		game.SetChat(chat);
		m_db.update(game);
		return true;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "Exception occurred while setting game chat: " << exception.what() << "\n";
		return false;
	}
}

bool Database::AddPlayerToGame(const Player& player, const std::string& roomID, int currentPlayers)
{
	try
	{
		auto existingGames = m_db.get_all<Game>(
			sql::where(sql::c(&Game::GetGameCode) == roomID)
		);

		if (existingGames.empty())
			return false;

		auto& game = existingGames[0];
		game.AddPlayer(player);
		game.SetCurrentPlayers(currentPlayers);
		m_db.update(game);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding player to game: " << e.what() << "\n";
		return false;
	}
}

bool Database::RemovePlayerFromGame(const Player& player, const std::string& roomID)
{
	try
	{
		auto existingGames = m_db.get_all<Game>(
			sql::where(sql::c(&Game::GetGameCode) == roomID)
		);

		if (existingGames.empty())
			return false;

		auto& game = existingGames[0];
		game.RemovePlayer(player);
		m_db.update(game);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while removing player from game: " << e.what() << "\n";
		return false;
	}
}

bool Database::CheckUsername(const std::string& username)
{
	try
	{
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::GetName) == username)
		);

		return !existingPlayers.empty();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while checking if username exists: " << e.what() << "\n";
		return false;
	}
}

bool Database::CheckRoomID(const std::string& roomID)
{
	try
	{
		auto existingGames = m_db.get_all<Game>(
			sql::where(sql::c(&Game::GetGameCode) == roomID)
		);

		return !existingGames.empty();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while checking if roomID exists: " << e.what() << "\n";
		return false;
	}
}

bool Database::CheckPassword(const std::string& username, const std::string& password)
{
	try
	{
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::GetName) == username)
		);

		if (existingPlayers.empty())
			return false;

		return existingPlayers[0].GetPassword() == password;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while checking if password is correct: " << e.what() << "\n";
		return false;
	}
}

Game Database::GetGame(const std::string& roomID)
{
	auto existingGames = m_db.get_all<Game>(
		sql::where(sql::c(&Game::GetGameCode) == roomID)
	);
	return existingGames[0];
}

Round Database::GetRound(const std::string& roomID)
{
	auto existingRounds = m_db.get_all<Round>(
		sql::where(sql::c(&Round::GetGameId) == roomID)
	);
	return existingRounds[0];
}

Player Database::GetPlayer(const std::string& username)
{
	try
	{
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::GetName) == username)
		);

		if (existingPlayers.empty())
			return Player{};

		return existingPlayers[0];
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while getting player: " << e.what() << "\n";
		return Player{};
	}
}

int Database::GetPlayerScore(const std::string& username)
{
	auto existingPlayers = m_db.get_all<Player>(
		sql::where(sql::c(&Player::GetName) == username)
	);

	if (existingPlayers.empty())
		return 0;

	return existingPlayers[0].GetPoints();
}

void Database::SetPlayerScore(const std::string& username, int score)
{
	try
	{
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::GetName) == username)
		);

		if (existingPlayers.empty())
			return;

		auto& player = existingPlayers[0];
		player.SetPoints(score);
		m_db.update(player);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while setting player score: " << e.what() << "\n";
	}
}

std::string Database::GetRandomID()
{
	std::string id = "";
	for (uint8_t i = 0; i < 5; i++)
		id += std::to_string(GenerateRandomNumber(0, 9));

	return id;
}

void Database::PopulateStorage()
{
	std::vector<Word> words;
	std::string word;
	std::ifstream input{ "words.txt" };
	if (input.fail())
	{
		std::cerr << "Failed to open words.txt\n";
		return;
	}
	while (std::getline(input, word))
		words.emplace_back(Word{ -1, std::move(word) });

	input.close();
	m_db.insert_range(words.begin(), words.end());
}

int Database::GenerateRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}