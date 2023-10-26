#include "Player.h"

Player Player::getPlayerDetails()
{
	return *this;
}

void Player::editPlayerPoints(int points)
{
	this->m_points = points;
}

Player::Player()
{
	this->m_name = "noname";
	this->m_points = 0;
}

Player::Player(const std::string& name)
	: m_name (name),
	m_points(0)
{
}

Player::Player(const Player& player)
{
	this->m_name = player.m_name;
	this->m_points = player.m_points;
}


bool Player::operator<(const Player& player)
{
	return this->m_points < player.m_points;
}
