export module leaderboard;

import player;

import <vector>;
import <set>;


namespace skribbl
{
    export class Leaderboard {
    public:
        Leaderboard(const std::vector<Player>& players);
        const std::set<Player> getLeaderboard() const noexcept;
        void setLeaderboard(const std::vector<Player>& players);
    private:
        std::set<Player> m_scoreboard;
};
}
