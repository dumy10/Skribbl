import player;

using namespace skribbl;

void Player::setPlayerPoints(int points)
{
	this->m_points = points;
}

int Player::getPlayerPoints() const
{
	return this->m_points;
}

const std::string_view Player::getPlayerName() const noexcept
{
	return this->m_name;
}

const std::string_view Player::getPlayerPassword() const noexcept
{
	return this->m_password;
}


Player::Player(const std::string& name, const std::string& password, const std::string& email) :
	m_name{ name },
	m_password{ password },
	m_email{ email },
	m_points{ 0 }
{
	// Empty
}

Player::Player(const Player& player)
{
	this->m_name = player.m_name;
	this->m_password = player.m_password;
	this->m_email = player.m_email;
	this->m_points = player.m_points;
}


bool Player::operator<(const Player& player)
{
	return this->m_points < player.m_points;
}
