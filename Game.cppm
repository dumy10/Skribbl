export module game;

import leaderboard;
import player;
import round;

import <vector>;


namespace skribbl 
{
	export class Game {
    public:
        Game(const std::vector<Player>& players);
        void startGame();
        void endGame();
        Player getWinner(const Leaderboard& leaderboard) const;
        void login(const Player& player);

    private:
        std::vector<Player> m_players;
        std::vector<Round> m_rounds;
        static const size_t kNoOfRounds{ 4 };
	};
}