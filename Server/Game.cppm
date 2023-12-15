export module game;

export import round;
export import player;
import <vector>;


namespace skribbl
{
	export class Game {
	public:
		Game() = default;

		void addPlayer(const Player& player);
		void removePlayer(const Player& player);

		void startGame();
		void endGame();
		//Player getWinner() const;
		void makeAdmin(const Player& player);
		void removeAdmin(const Player& player);

	private:
		std::vector<Player> m_players;
		std::vector<Round> m_rounds;
		static const size_t kNoOfRounds{ 4 };
		size_t m_maxPlayers;

		//leaderboard should come here and m_roundNumber should come here also . We should remember only one round , not a vector of rounds
	};
}