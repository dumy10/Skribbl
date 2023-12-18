module game;

using namespace skribbl;


Game::Game(int id, const Player& player, const std::string& gameCode, size_t maxPlayers, size_t currentPlayers)
	: m_id{ id },
	m_gameCode{ gameCode },
	m_maxPlayers{ maxPlayers }
{
	this->m_players.push_back(player);
	m_gameStatus = GameStatus::WAITING;
}

void Game::StartGame()
{
	/*
	- check if there are enough players
	- check if game state is waiting, switch to INPROGRESS
	- need to create a round (4 rounds in total)
	- need to set the first player to draw
	- need to set the first word
	- provide the word to the player who draws and start the timer, provide the length of the word to the other players
	- set a timer for the round (60 seconds)
	- while the timer is running, players can guess the word
	- if the word is guessed, the player who guessed it gets points based on the time left
	- after the timer runs out need to switch to the next player and set a new word
	- after 4 rounds, the game ends and the player with the most points wins
	*/

}

void Game::EndGame()
{

}

void Game::AddPlayer(const Player& player)
{
	this->m_players.push_back(player);
}

void Game::RemovePlayer(const Player& player)
{
	for (size_t i = 0; i < this->m_players.size(); i++)
	{
		if (this->m_players[i] == player)
		{
			this->m_players.erase(this->m_players.begin() + i);
			break;
		}
	}
}

std::string Game::GetGameCode() const noexcept
{
	return this->m_gameCode;
}

const Game::GameStatus Game::GetGameStatus() const noexcept
{
	return this->m_gameStatus;
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

int Game::GetGameStatusAsInt() const
{
	return static_cast<int>(m_gameStatus);
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

std::string Game::SerializePlayers() const
{
	std::string serializedPlayers;
	for (const auto& player : m_players)
		serializedPlayers += player.GetName() + ",";

	if (!serializedPlayers.empty())
		serializedPlayers.pop_back();

	return serializedPlayers;
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
