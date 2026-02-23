#include "database.h"

bool Database::Initialize()
{
	m_db.sync_schema();
	int initWordCount = m_db.count<Word>();

	if (initWordCount == 0) {
		PopulateStorage();
	}
		
	int wordCount = m_db.count<Word>();
	return wordCount != 0;
}

std::string Database::GetRandomWord()
{
	std::vector<Word> words{ m_db.get_all<Word>() };

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(words.begin(), words.end(), rd);
	std::shuffle(words.begin(), words.end(), rd);

	int randomWordIndex = GenerateRandomNumber(0, static_cast<int>(words.size() - 1));
	auto& word = words.at(randomWordIndex);

	return word.GetWord();
}

std::optional<std::shared_ptr<Player>>Database::AddUser(const std::string& username, const std::string& password, const std::string& email)
{
	try
	{
		std::string correctEmail{ email };
		m_db.insert(Player{ -1, username, password, email });

		return GetPlayer(username);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding user: " << e.what() << "\n";
		return std::nullopt;
	}
}

std::optional<std::shared_ptr<Game>> Database::AddGame(const Player& player, const std::string& gameCode, size_t maxPlayers)
{
	try
	{
		m_db.insert(Game{ -1, player, gameCode, maxPlayers, 1 });
		return GetGame(gameCode);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding game: " << e.what() << "\n";
		return std::nullopt;
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

		if (existingPlayers.empty()) {
			return false;
		}

		return existingPlayers.front().GetPassword() == password;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while checking if password is correct: " << e.what() << "\n";
		return false;
	}
}

std::optional<std::shared_ptr<Game>> Database::GetGame(const std::string& roomID)
{
	auto existingGames = m_db.get_all<Game>(
		sql::where(sql::c(&Game::GetGameCode) == roomID)
	);

	if (existingGames.empty()) {
		return std::nullopt;
	}

	return std::make_shared<Game>(std::move(existingGames.front()));
}

std::optional<std::shared_ptr<Player>> Database::GetPlayer(const std::string& username)
{
	try
	{
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::GetName) == username)
		);

		if (existingPlayers.empty()) {
			return std::nullopt;
		}

		return std::make_shared<Player>(std::move(existingPlayers.front()));
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while getting player: " << e.what() << "\n";
		return std::nullopt;
	}
}

std::vector<std::shared_ptr<Player>> Database::GetAllPlayers()
{
	std::vector<std::shared_ptr<Player>> players;
	try
	{
		auto allPlayers = m_db.get_all<Player>();
		for (auto& player : allPlayers) {
			players.push_back(std::make_shared<Player>(std::move(player)));
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while getting all players: " << e.what() << "\n";
	}

	return players;
}

std::string Database::GetRandomID()
{
	std::string id = "";
	for (uint8_t i = 0; i < 5; i++) {
		id += std::to_string(GenerateRandomNumber(0, 9));
	}

	return id;
}

void Database::PopulateStorage()
{
	std::vector<Word> words;
	words.reserve(500); // Reserve space for 500 words to avoid multiple allocations
	std::ifstream input{ "words.txt" };
	if (input.fail()) {
		std::cerr << "Failed to open words.txt\n";
		return;
	}

	std::string word;
	while (std::getline(input, word)) {
		words.emplace_back(Word{ -1, std::move(word) });
	}

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