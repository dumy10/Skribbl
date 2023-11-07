export module game;

export import leaderboard;
export import round;
export import player;
export import wordgenerator;
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