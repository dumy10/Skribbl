#include "database.h"

#include <fstream>


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
	for (std::ifstream in{ "words.txt" }; !in.eof(); /*EMPTY*/)
	{
		in >> word;
		words.emplace_back(Word{ -1, std::move(word) });
	}
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


int Database::GenerateRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}
