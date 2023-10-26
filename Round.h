#pragma once
#include<string>
#include<map>
#include "Player.h"
class Round
{
public:
	void startRound();
	void endRound();
	void cleanBoard();
	std::string generateCharacters(const std::string& currentWord);
	//Leaderboard updateLeaderboard(const Leaderboard& currentLeaderboard);
	void draw(); //to be implemented, needs GUI
	bool guessWord(std::string word);
private:
	//Leaderboard currentLeaderboard;
	std::string currentWord;

};

