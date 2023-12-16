module game;
import turn;
import <iostream>;
import <string>;
import <fstream>;
import <sstream>;
import <vector>;


using namespace skribbl;



Game::Game(int id, const Player& player, const std::string& gameCode, size_t maxPlayers)
	: m_id{ id },
	m_gameCode{ gameCode },
	m_maxPlayers{ maxPlayers }
{
	this->m_players.push_back(player);
	m_gameStatus = GameStatus::Waiting;
}

void Game::addPlayer(const Player& player)
{
	this->m_players.push_back(player);
}

void Game::removePlayer(const Player& player)
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

/*
startGame function needs rethinking
*/

void Game::startGame()
{

}

void Game::endGame()
{
	//clearScreen(); function not declared
	std::cout << "The winner is : "; //show the first place <<
	std::cout << "Points : ";//show the points 
	std::cout << "\n Seccond place : ";
	std::cout << "Points : ";
	std::cout << "\n Third place : ";
	std::cout << "Points:";
	std::cout << "\n Play again?";
	std::cout << "Leave";//make them buttons in the future
}

std::string Game::getGameCode() const noexcept
{
	return this->m_gameCode;
}

const Game::GameStatus Game::getGameStatus() const noexcept
{
	return this->m_gameStatus;
}

void Game::setGameCode(const std::string& gameCode)
{
	this->m_gameCode = gameCode;
}

void Game::setGameStatus(GameStatus status)
{
	this->m_gameStatus = status;
}

void Game::setMaxPlayers(size_t maxPlayers)
{
	this->m_maxPlayers = maxPlayers;
}

void Game::setCurrentPlayers(int currentPlayers)
{
	// setter fictiv
}

void Game::setId(int id)
{
	this->m_id = id;
}

void Game::setPlayers(const std::vector<Player>& players)
{
	this->m_players = players;
}

void Game::setGameStatusInt(int status)
{
	this->m_gameStatus = static_cast<GameStatus>(status);
}


int Game::getGameStatusAsInt() const
{
	return static_cast<int>(m_gameStatus);
}

void Game::deserializePlayers(const std::string& serializedPlayers) {
	m_players.clear();
	std::istringstream iss(serializedPlayers);
	std::string playerName;
	while (std::getline(iss, playerName, ',')) {
		m_players.push_back(Player{ 0, playerName, "", "" }); // Assuming default values for other parameters
	}
}

int Game::getCurrentPlayers() const
{
	return m_players.size();
}

std::string Game::serializePlayers() const
{
	std::string serializedPlayers;
	for (const auto& player : m_players) {
		serializedPlayers += player.getName() + ",";
	}
	if (!serializedPlayers.empty()) {
		serializedPlayers.pop_back();
	}
	return serializedPlayers;
}

const size_t Game::getMaxPlayers() const noexcept
{
	return this->m_maxPlayers;
}

const std::vector<Player>& skribbl::Game::getPlayers() const noexcept
{
	return this->m_players;
}

int Game::getId() const noexcept
{
	return this->m_id;
}
