module game;

using namespace skribbl;


Game::Game(int id, const Player& player, const std::string& gameCode, size_t maxPlayers, size_t currentPlayers)
	: m_id{ id },
	m_gameCode{ gameCode },
	m_maxPlayers{ maxPlayers }
{
	this->m_players.push_back(player);
	m_gameStatus = GameStatus::WAITING;
	m_chat = "Welcome to skribbl!\n";
}

void Game::AddPlayer(const Player& player)
{
	this->m_players.push_back(player);
}

void Game::RemovePlayer(const Player& player)
{
	auto playerIt = std::find(this->m_players.begin(), this->m_players.end(), player);
	if (playerIt != this->m_players.end())
	{
		this->m_players.erase(playerIt);
		this->m_currentPlayers--;
	}
}

void Game::DeserializePlayers(const std::string& serializedPlayers) {
	m_players.clear();
	std::istringstream iss(serializedPlayers);
	std::string playerName;
	while (std::getline(iss, playerName, ','))
		m_players.push_back(Player{ 0, playerName, "", "" }); // Assuming default values for other parameters
}

int Game::GetCurrentPlayers() const
{
	return m_players.size();
}

int Game::GetGameStatusAsInt() const
{
	return static_cast<int>(m_gameStatus);
}

std::string Game::SerializePlayers() const
{
	std::string serializedPlayers;
	for (const auto& player : m_players)
		serializedPlayers += player.GetName() + ",";

	if (!serializedPlayers.empty())
		serializedPlayers.pop_back();

	return serializedPlayers;
}

std::string Game::GetGameCode() const noexcept
{
	return this->m_gameCode;
}

const Game::GameStatus Game::GetGameStatus() const noexcept
{
	return this->m_gameStatus;
}

const size_t Game::GetMaxPlayers() const noexcept
{
	return this->m_maxPlayers;
}

const std::vector<Player>& Game::GetPlayers() const noexcept
{
	return this->m_players;
}

int Game::GetId() const noexcept
{
	return this->m_id;
}

int Game::GetPlayerScore(const std::string& username) const noexcept
{
	for (const auto& player : m_players)
		if (player.GetName() == username)
			return player.GetPoints();
	return 0;
}

int Game::GetNoOfRounds() const noexcept
{
	return kNoOfRounds;
}

std::string Game::GetChat() const noexcept
{
	return m_chat;
}

std::string Game::GetDrawingPlayer() const noexcept
{
	return m_round.GetDrawingPlayer();
}

uint8_t Game::GetRoundNumber() const noexcept
{
	return m_round.GetRoundNumber();
}

int Game::GetPlayerIndex(const std::string& username) const noexcept
{
	for (uint8_t i = 0; i < m_players.size(); i++)
		if (m_players[i].GetName() == username)
			return i;
	return -1;
}

void Game::SetGameCode(const std::string& gameCode)
{
	this->m_gameCode = gameCode;
}

void Game::SetGameStatus(GameStatus status)
{
	this->m_gameStatus = status;
}

void Game::SetMaxPlayers(size_t maxPlayers)
{
	this->m_maxPlayers = maxPlayers;
}

void Game::SetCurrentPlayers(int currentPlayers)
{
	this->m_currentPlayers = currentPlayers;
}

void Game::SetId(int id)
{
	this->m_id = id;
}

void Game::SetPlayers(const std::vector<Player>& players)
{
	this->m_players = players;
}

void Game::SetGameStatusInt(int status)
{
	this->m_gameStatus = static_cast<GameStatus>(status);
}

void Game::SetChat(const std::string& chat)
{
	this->m_chat = chat;
}
