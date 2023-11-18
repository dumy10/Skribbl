module leaderboard;
import player;
import <algorithm>;

using namespace skribbl;

Leaderboard::Leaderboard(const std::vector<Player>& players)
{
	for (Player player : players) 
		this->m_scoreboard.emplace_back(player);
	std::sort(this->m_scoreboard.begin(), this->m_scoreboard.end(), std::greater<>());

}

const std::vector<Player> Leaderboard::getLeaderboard() const noexcept
{
	return this->m_scoreboard;
}

void Leaderboard::setLeaderboard(const std::vector<Player>& players)
{
	if(!this->m_scoreboard.empty())
		this->m_scoreboard.clear();
	for (Player player : players)
		this->m_scoreboard.emplace_back(player);
	std::sort(this->m_scoreboard.begin(), this->m_scoreboard.end(), std::greater<>());

}
