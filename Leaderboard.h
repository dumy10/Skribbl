#pragma once
#include"Player.h"
#include<set>
class Leaderboard
{
public:
	Leaderboard(); 
	std::set<Player> getLeaderboard();
private:
	std::set<Player> scoreboard;
};

