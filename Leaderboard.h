#pragma once
#include"Player.h"
#include <list>
#include<set>
class Leaderboard
{
public:
	Leaderboard(const std::list<Player>& players); 
	std::set<Player> getLeaderboard() const;
private:
	std::set<Player> m_scoreboard;
};

