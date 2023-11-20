module player;

using namespace skribbl;

Player::Player()
{
	m_name = std::string();
	m_password = std::string();
	m_email = std::string();
	m_points = 0;

}

Player::Player(const std::string& name, const std::string& password, const std::string& email) :
	m_name{ name },
	m_password{ password },
	m_email{ email },
	m_points{ 0 }
{
	// Empty
}



void Player::setId(int id)
{
	this->m_id = id;
}

void Player::setName(const std::string& name)
{
	this->m_name = name;
}

void Player::setPassword(const std::string& password)
{
	this->m_password = password;
}

void Player::setEmail(const std::string& email)
{
	this->m_email = email;
}

void Player::setPoints(int points)
{
	this->m_points = points;
}

void Player::addPoints(int points)
{
	this->m_points += points;
}

int Player::getId() const
{
	return this->m_id;

}

int Player::getPoints() const
{
	return this->m_points;
}

const std::string Player::getName() const noexcept
{
	return this->m_name;
}

const std::string Player::getPassword() const noexcept
{
	return this->m_password;
}

const std::string Player::getEmail() const noexcept
{
	return this->m_email;
}

bool Player::operator<(const Player& player)
{
	return this->m_points < player.m_points;
}

bool Player::operator>(const Player& player)
{
	return this->m_points > player.m_points;
}

bool Player::operator==(const Player& player)
{
	return this->m_name == player.m_name && this->m_points == player.m_points;
}