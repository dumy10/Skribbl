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

std::string Database::GetRandomWord()
{
	auto wordCount = m_db.count<Word>();
	auto randomWordIndex = GenerateRandomNumber(1, wordCount);
	auto word = m_db.get<Word>(randomWordIndex);
	return word.GetWord();
}

std::vector<Word> Database::GetWords()
{
	return m_db.get_all<Word>();
}

std::vector<std::string> Database::GetCustomNumberOfWords(int numberOfWords)
{
	std::vector<std::string> words;
	auto wordCount = m_db.count<Word>();

	for (int i = 0; i < numberOfWords; i++)
	{
		auto randomWordIndex = GenerateRandomNumber(1, wordCount);
		auto word = m_db.get<Word>(randomWordIndex);
		words.push_back(word.GetWord());
	}

	return words;
}

std::vector<Game> Database::GetGames()
{
	return m_db.get_all<Game>();
}

std::vector<Player> Database::GetPlayers()
{
	return m_db.get_all<Player>();
}

bool Database::AddUser(const std::string& username, const std::string& password, const std::string& email)
{
	try
	{
		m_db.insert(Player{ -1, username, password, email });
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
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding game: " << e.what() << "\n";
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

bool Database::AddPlayerToGame(const Player& player, const std::string& roomID, int currentPlayers)
{
	try
	{
		auto existingGames = m_db.get_all<Game>(
			sql::where(sql::c(&Game::GetGameCode) == roomID)
		);

		if (existingGames.empty())
			return false;

		auto game = existingGames[0];
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

Player Database::GetPlayer(const std::string& username)
{
	auto existingPlayers = m_db.get_all<Player>(
		sql::where(sql::c(&Player::GetName) == username)
	);
	return existingPlayers[0];
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

std::string Database::GetRandomID()
{
	std::string id = "";
	for (uint8_t i = 0; i < 5; i++)
		id += std::to_string(GenerateRandomNumber(0, 9));

	return id;
}

int Database::GenerateRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}
