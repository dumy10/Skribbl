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
	while (std::getline(input, word))
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


bool Database::AddUser(const std::string& username, const std::string& password, const std::string& email)
{
	try {
		m_db.insert(Player{ -1, username, password, email});
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while adding user: " << e.what() << "\n";
		return false;
	}
}

bool Database::CheckUsername(const std::string& username)
{
	try {
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::getName) == username)
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
	try {
		auto existingPlayers = m_db.get_all<Player>(
			sql::where(sql::c(&Player::getName) == username)
		);

		if (existingPlayers.empty())
			return false;

		return existingPlayers[0].getPassword() == password;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while checking if password is correct: " << e.what() << "\n";
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
