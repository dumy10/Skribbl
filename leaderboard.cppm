export module Leaderboard;

import Player;
using modern::Player;
import <list>;
import <set>;

namespace modern{
    export class Leaderboard {
    public:
        Leaderboard(const std::list<Player>& players);
        std::set<Player> getLeaderboard() const;
    private:
        std::set<Player> m_scoreboard;
};
}
