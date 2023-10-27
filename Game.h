#pragma once
#include "Player.h"
#include "Leaderboard.h"
class Game
{
public:
	void startGame();
	void endGame();
	Player getWinner(const Leaderboard& leaderboard) const;
	void login(const Player& player);
private:
	std::list<Player> m_players;
	static const size_t kNoOfRounds{ 4 };
};

