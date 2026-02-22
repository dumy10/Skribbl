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

const int Player::GetId() const noexcept
{
	return m_id;
}

const int Player::GetPoints() const noexcept
{
	return m_points;
}

const std::string Player::GetName() const noexcept
{
	return m_name;
}

const std::string Player::GetPassword() const noexcept
{
	return m_password;
}

const std::string Player::GetEmail() const noexcept
{
	return m_email;
}

const std::string skribbl::Player::Serialize() const noexcept
{
	return std::to_string(m_id) + "," + m_name + "," + m_password + ","
		+ m_email + "," + std::to_string(m_points);
}

const Player skribbl::Player::Deserialize(const std::string& serializedPlayer) const
{
	std::istringstream ss(serializedPlayer);
	std::string token;
	std::vector<std::string> tokens;
	while (std::getline(ss, token, ',')) {
		tokens.push_back(token);
	}

	if (tokens.size() != 5) {
		throw std::runtime_error("Invalid serialized player format");
	}

	Player deserializedPlayer;
	deserializedPlayer.SetId(std::stoi(tokens[0]));
	deserializedPlayer.SetName(tokens[1]);
	deserializedPlayer.SetPassword(tokens[2]);
	deserializedPlayer.SetEmail(tokens[3]);
	deserializedPlayer.SetPoints(std::stoi(tokens[4]));

	return deserializedPlayer;
}

void Player::SetId(int id)
{
	m_id = id;
}

void Player::SetName(const std::string& name)
{
	m_name = name;
}

void Player::SetPassword(const std::string& password)
{
	m_password = password;
}

void Player::SetEmail(const std::string& email)
{
	m_email = email;
}

void Player::SetPoints(int points)
{
	m_points = points;
}

void Player::AddPoints(int points)
{
	m_points += points;
}

bool Player::operator<(const Player& player)
{
	return m_points < player.m_points;
}

bool Player::operator>(const Player& player)
{
	return m_points > player.m_points;
}

bool Player::operator==(const Player& player)
{
	return m_name == player.m_name;
}