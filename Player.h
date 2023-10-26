#pragma once
#include<string>
class Player
{
public:
	Player getPlayerDetails();
	void editPlayerPoints(int points);
	Player();
	Player(std::string nume);
	Player(const Player& p1);
	~Player();
	bool operator<(const Player& p1);

private:
	std::string name;
	int points;

};

