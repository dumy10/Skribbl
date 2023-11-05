import game; 
import player;
import wordgenerator;
import round;
import leaderboard;
import turn;
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
	
	/*
		Logic on how the rounds are initiallized needs to be reworked

	this->m_rounds.resize(kNoOfRounds);
	uint8_t roundNumber{ 1 };
	for(Round& round : this->m_rounds)
		round = Round(Leaderboard(this->m_players), Turn(this->m_players[0]), WordGenerator::generateWords(), roundNumber++, players);
	*/
}


Player Game::getWinner(const Leaderboard& leaderboard) const
{
	if(leaderboard.getLeaderboard().size() == 0)
		return Player();
	else
		return leaderboard.getLeaderboard()[0];
}
