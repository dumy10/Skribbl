#pragma once
#include "Player.h"
#include "Leaderboard.h"
#include<list>
class Game
{
public:
	void startGame();
	void endGame();
	Player getWinner(const Leaderboard leaderboard);
	void login(Player player);
private:
	std::list<Player> m_players;
	const static int noOfRounds = 4;
};

