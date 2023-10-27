#include "Player.h"

void Player::setPlayerPoints(int points)
{
	this->m_points = points;
}

int Player::getPlayerPoints() const
{
	return this->m_points;
}

std::string Player::getPlayerName() const
{
	return this->m_name;
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
