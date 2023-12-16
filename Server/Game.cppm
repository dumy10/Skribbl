export module game;

export import round;
export import player;
import <vector>;
import <string>;
import <array>;


namespace skribbl
{
	export class Game {
	public:
		enum class GameStatus : uint8_t
		{
			Unknown,
			Waiting,
			InProgress,
			Finished
		};

	public:
		Game() = default;
		Game(int id, const Player& player, const std::string& gameCode, size_t maxPlayers);
		void addPlayer(const Player& player);
		void removePlayer(const Player& player);
		void startGame();
		void endGame();
		void deserializePlayers(const std::string& serializedPlayers);
		int getCurrentPlayers() const;
		int getGameStatusAsInt() const;
		std::string serializePlayers() const;

		std::string getGameCode() const noexcept;
		const GameStatus getGameStatus() const noexcept;
		const size_t getMaxPlayers() const noexcept;
		const std::vector<Player>& getPlayers() const noexcept;
		int getId() const noexcept;

		void setGameCode(const std::string& gameCode);
		void setGameStatus(GameStatus status);
		void setMaxPlayers(size_t maxPlayers);
		void setCurrentPlayers(int currentPlayers);
		void setId(int id);
		void setPlayers(const std::vector<Player>& players);
		void setGameStatusInt(int status);
		//Player getWinner() const;

	private:
		std::vector<Player> m_players;
		std::vector<Round> m_rounds;
		static const size_t kNoOfRounds{ 4 };
		size_t m_maxPlayers;
		std::string m_gameCode;
		GameStatus m_gameStatus{ GameStatus::Unknown };
		int m_id;


		//leaderboard should come here and m_roundNumber should come here also . We should remember only one round , not a vector of rounds
	};
}