import leaderboard;
using modern::Leaderboard;
Leaderboard::Leaderboard(const std::list<Player>& players)
{
	for (Player player : players) 
		this->m_scoreboard.insert(player);
}

std::set<Player> Leaderboard::getLeaderboard() const
{
	return this->m_scoreboard;
}
