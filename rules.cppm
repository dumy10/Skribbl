export module Rules;

import Player;
using modern::Player;

namespace modern {
    export class Rules {
    public:
     void getLeaderboardAfterRound();
        int getPlayerScore(const Player& player) const;
     void getDrawRules();
    };
}

