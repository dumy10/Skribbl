#pragma once
#include<string>
class Player
{
public:
	Player getPlayerDetails();
	void editPlayerPoints(int points);
	Player();
	Player(const std::string& name);
	Player(const Player& player);
	bool operator<(const Player& player);

private:
	std::string m_name;
	int m_points;
};

