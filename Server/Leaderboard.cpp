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

void skribbl::Leaderboard::addPoints(Player& player,const int& timeLeft)
{
	int points = 0;
	if (timeLeft > 30)
	{
		points = 100;
		player.addPoints(points);
	}
	else {
		if (timeLeft > 0)
		{
			points = int((60 - timeLeft) * 100 / 30);
			player.addPoints(points);
		}
		else {
			substractPoints(player);
		}
	}
}

void skribbl::Leaderboard::substractPoints(Player& player)
{
	int points = 0;
	points = 50;
	player.substractPoints(points);
}

void skribbl::Leaderboard::addPointsForTheDrawer(Player& player)
{
	int points=int((60-m_averageTime)*100/60);
	player.addPoints(points);
}

void skribbl::Leaderboard::substractPointsForTheDrawer(Player& player)
{
	int points = 0;
	points = 50;
	player.substractPoints(points);
}

void skribbl::Leaderboard::averageTime(const int& timeLeft)
{
	this->m_averageTime += 60-timeLeft;
	this->m_averageTime /= 2;
}

