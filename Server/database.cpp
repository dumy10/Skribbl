#include "database.h"

#include <fstream>
#include <iostream>


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
	while(std::getline(input,word))
	{
		words.emplace_back(Word{ -1, std::move(word) });
	}
	input.close();
	m_db.insert_range(words.begin(), words.end());
}

std::string Database::GetRandomWord()
{
	auto wordCount = m_db.count<Word>();
	auto randomWordIndex = GenerateRandomNumber(1, wordCount);
	auto word = m_db.get<Word>(randomWordIndex);
	return word.getWord();
}

std::vector<Word> Database::GetWords()
{
	return m_db.get_all<Word>();
}

std::vector<std::string> Database::GetCustomNumberOfWords(int numberOfWords)
{
	std::vector<std::string> words;
	auto wordCount = m_db.count<Word>();

	for (int i = 0; i < numberOfWords; ++i) {
		auto randomWordIndex = GenerateRandomNumber(1, wordCount);
		auto word = m_db.get<Word>(randomWordIndex);
		words.push_back(word.getWord());
	}

	return words;
}

bool Database::AddPlayer(const Player& player)
{
	try {
		if (PlayerExists(player)) {
			std::cerr << "Player with the same name or email already exists.\n";
			return false;
		}

		auto playerId = m_db.insert(player);

		if (playerId > 0) {
			std::cout << "Player added successfully with ID: " << playerId << "\n";
			return true;
		}
		else {
			std::cerr << "Failed to add player to the database.\n";
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding player: " << e.what() << "\n";
		return false;
	}
}

bool Database::PlayerExists(const Player& player) {
	try {
		auto existingPlayers1 = m_db.get_all<Player>(
			sql::where(sql::c(&Player::getName) == player.getName())
		);

		auto existingPlayers2 = m_db.get_all<Player>(
			sql::where(sql::c(&Player::getEmail) == player.getEmail())
		);

		return !existingPlayers1.empty() || !existingPlayers2.empty();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while checking if player exists: " << e.what() << "\n";
		return false;
	}
}


int Database::GenerateRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}
