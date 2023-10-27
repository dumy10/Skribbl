#pragma once
#include<string>
class Player
{
public:
	void setPlayerPoints(int points);
	int getPlayerPoints() const;
	std::string getPlayerName() const;
	Player();
	Player(const std::string& name);
	Player(const Player& player);
	bool operator<(const Player& player);

private:
	std::string m_name;
	int m_points;
};

