import game; 
import player;
import wordgenerator;
import round;
import leaderboard;
import turn;

using namespace skribbl;


Game::Game(const std::vector<Player>& players):
	m_players {players}
{
	this->m_rounds.resize(kNoOfRounds);
	uint8_t roundNumber{ 1 };
	/*
	Needs to generate a word for each round
	*/
	for(Round& round : this->m_rounds)
		round = Round(Leaderboard(this->m_players), Turn(this->m_players[0]), WordGenerator::generateWords(), roundNumber++, players);
}


Player Game::getWinner(const Leaderboard& leaderboard) const
{
	if(leaderboard.getLeaderboard().size() == 0)
		return Player();
	else
	{
		std::set<Player> players = leaderboard.getLeaderboard();
		return *std::max_element(players.begin(), players.end());
	}
}
