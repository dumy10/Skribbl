#pragma once
#include<string>
#include<vector>
class WordGenerator {
public:
	std::string generateWords();
	std::vector<std::string> lexicalFamily(const std::string& word);

private:
	std::vector<std::string> m_usedWords;
};