#pragma once
#include<string>
#include<vector>
class WordGenerator {
public:
	std::string generateWords();
	std::vector<std::string> lexicalFamily(std::string word);

private:
	std::vector<std::string> usedWords;
};