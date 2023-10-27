#pragma once
#include"Player.h"

class Rules 
{
public:

	void getLeaderboardAfterRound();
	int getPlayerScore(const Player& player) const;
	void getDrawRules();
};