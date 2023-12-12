export module leaderboard;

import player;

import <vector>;
import <set>;


namespace skribbl
{
    export class Leaderboard {
    public:
        Leaderboard(const std::vector<Player>& players);
        const std::vector<Player> getLeaderboard() const noexcept;
        void setLeaderboard(const std::vector<Player>& players);
        void addPoints(Player& player,const int& timeLeft);
        void substractPoints(Player& player);
        void addPointsForTheDrawer(Player& player);
        void substractPointsForTheDrawer(Player& player);
        void averageTime(const int& timeLeft);

    private:
        std::vector<Player> m_scoreboard;
        float m_averageTime=0;
};
}
