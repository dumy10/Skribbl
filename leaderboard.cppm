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
    private:
        std::vector<Player> m_scoreboard;
};
}
