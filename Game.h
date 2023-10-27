#pragma once
#include "Player.h"
#include "Leaderboard.h"
class Game : public Leaderboard
{
public:
	void startGame();
	void endGame();
	Player getWinner(const Leaderboard& leaderboard) const;
	void login(const Player& player);
private:
	std::list<Player> m_players;
	const static uint8_t m_noOfRounds = 4;
};

