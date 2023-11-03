import Leaderboard;
using modern::Leaderboard;
import Player;
using modern::Player;
Leaderboard::Leaderboard(const std::list<Player>& players)
{
	for (Player player : players) 
		this->m_scoreboard.insert(player);
}

std::set<Player> Leaderboard::getLeaderboard() const
{
	return this->m_scoreboard;
}
