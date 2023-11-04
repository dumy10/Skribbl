import leaderboard;
import player;

using namespace skribbl;

Leaderboard::Leaderboard(const std::vector<Player>& players)
{
	for (Player player : players) 
		this->m_scoreboard.insert(player);
}

const std::set<Player> Leaderboard::getLeaderboard() const noexcept
{
	return this->m_scoreboard;
}

void Leaderboard::setLeaderboard(const std::vector<Player>& players)
{
	this->m_scoreboard.clear();
	for (Player player : players)
		this->m_scoreboard.insert(player);
}
