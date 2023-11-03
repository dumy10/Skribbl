#pragma once
import Player;
using modern::Player;
class Rules 
{
public:

	void getLeaderboardAfterRound();
	int getPlayerScore(const Player& player) const;
	void getDrawRules();
};