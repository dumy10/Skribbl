module game; 
import turn;
import <iostream>;
import <string>;
import <fstream>;

using namespace skribbl;


Game::Game(const std::vector<Player>& players):
	m_players{ players }
{

	/*
	Needs to generate a word for each round using random_shuffle
	Need to end the turn and move to the next player
	*/
	for(size_t i = 0; i < kNoOfRounds; i++)
		this->m_rounds.push_back(Round(Leaderboard(this->m_players), Turn(this->m_players[0], WordGenerator::generateWord()), WordGenerator::generateWord(), i + 1, players));
}

void Game::addPlayer(const Player& player)
{
	this->m_players.push_back(player);
}

void Game::removePlayer(const Player& player)
{
	for(size_t i = 0; i < this->m_players.size(); i++)
	{
		if(this->m_players[i] == player)
		{
			this->m_players.erase(this->m_players.begin() + i);
			break;
		}
	}
}

Player Game::getWinner(const Leaderboard& leaderboard) const
{
	if(leaderboard.getLeaderboard().size() == 0)
		return Player();
	else
		return leaderboard.getLeaderboard()[0];
}
