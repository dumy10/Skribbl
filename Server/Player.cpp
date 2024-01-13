module player;

using namespace skribbl;

Player::Player(int id, const std::string& name, const std::string& password, const std::string& email) :
	m_id{ id },
	m_name{ name },
	m_password{ password },
	m_email{ email },
	m_points{ 0 }
{
	// Empty
}

const std::string Player::GetName() const noexcept
{
	return this->m_name;
}

const std::string Player::GetPassword() const noexcept
{
	return this->m_password;
}

const std::string Player::GetEmail() const noexcept
{
	return this->m_email;
}

int Player::GetId() const
{
	return this->m_id;
}

int Player::GetPoints() const
{
	return this->m_points;
}

void Player::SetId(int id)
{
	this->m_id = id;
}

void Player::SetName(const std::string& name)
{
	this->m_name = name;
}

void Player::SetPassword(const std::string& password)
{
	this->m_password = password;
}

void Player::SetEmail(const std::string& email)
{
	this->m_email = email;
}

void Player::SetPoints(int points)
{
	this->m_points = points;
}

void Player::AddPoints(int points)
{
	this->m_points += points;
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
	return this->m_name == player.m_name;
}