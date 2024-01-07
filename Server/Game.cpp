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

void Game::StartGame(std::set<std::string> words)
{
	/*
	- need to set the first player to draw
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
	auto playerIt = std::find(this->m_players.begin(), this->m_players.end(), player);
	if (playerIt != this->m_players.end())
	{
		this->m_players.erase(playerIt);
		this->m_currentPlayers--;
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

void Game::SetChat(const std::string& chat)
{
	this->m_chat = chat;
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

void Game::AddPoints(Player& player, const int& timeLeft)
{
	int points = 0;
	if (timeLeft > 30)
	{
		points = 100;
		player.AddPoints(points);
	}
	else {
		if (timeLeft > 0)
		{
			points = int((60 - timeLeft) * 100 / 30);
			player.AddPoints(points);
		}
		else {
			SubstractPoints(player);
		}
	}
}

void Game::SubstractPoints(Player& player)
{
	player.SubstractPoints(50);
}

void Game::AddPointsForTheDrawer(Player& player)
{
	int points = int((60 - m_averageTime) * 100 / 60);
	player.AddPoints(points);
}

void Game::SubstractPointsForTheDrawer(Player& player)
{
	player.SubstractPoints(100);
}

void Game::AverageTime(const int& timeLeft)
{
	this->m_averageTime += 60 - timeLeft;
	this->m_averageTime /= 2;
}

void Game::SetPlayerScore(const std::string& username, int score)
{
	for (auto& player : m_players)
		if (player.GetName() == username)
			player.SetPoints(score);
}