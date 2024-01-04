export module game;

import round;
import player;
import <vector>;
import <string>;
import <array>;
import <sstream>;
import <algorithm>;


namespace skribbl
{
	export class Game {
	private:
		enum class GameStatus : uint8_t
		{
			UNKNOWN,
			WAITING,
			INPROGRESS,
			FINISHED
		};

	public:
		Game() = default;
		Game(int id, const Player& player, const std::string& gameCode, size_t maxPlayers, size_t currentPlayers);

		void StartGame();
		void EndGame();

		void AddPlayer(const Player& player);
		void RemovePlayer(const Player& player);

		void DeserializePlayers(const std::string& serializedPlayers);
		int GetCurrentPlayers() const;
		int GetGameStatusAsInt() const;
		std::string SerializePlayers() const;
		std::string SerializePlayersForLeaderboard() const;
		std::string GetGameCode() const noexcept;
		const GameStatus GetGameStatus() const noexcept;
		const size_t GetMaxPlayers() const noexcept;
		const std::vector<Player>& GetPlayers() const noexcept;
		int GetId() const noexcept;
		int GetPlayerScore(const std::string& username) const noexcept;

		void SetGameCode(const std::string& gameCode);
		void SetGameStatus(GameStatus status);
		void SetMaxPlayers(size_t maxPlayers);
		void SetCurrentPlayers(int currentPlayers);
		void SetId(int id);
		void SetPlayers(const std::vector<Player>& players);
		void SetGameStatusInt(int status);
		//const Player& getWinner() const;

		void AddPoints(Player& player, const int& timeLeft);
		void SubstractPoints(Player& player);
		void AddPointsForTheDrawer(Player& player);
		void SubstractPointsForTheDrawer(Player& player);
		void AverageTime(const int& timeLeft);
		uint8_t GetCurrentRound() const;

		const std::string GetWord() const noexcept;
		std::string GetDrawer() const;

		void SetPlayerScore(const std::string& username, int score);

	private:
		int m_id;
		static const size_t kNoOfRounds{ 4 };
		size_t m_currentPlayers;
		size_t m_maxPlayers;
		std::string m_gameCode;
		std::vector<Player> m_players; // we need to sort the players by their points and the map would not be a good choice
		std::vector<Round> m_rounds; // is a vector needed? maybe a single round is enough and change/modify round after each round ends
		//we should not remember the round at all.It will be created when the game starts.
		GameStatus m_gameStatus{ GameStatus::UNKNOWN };

		uint8_t m_currentRound{ 0 };

		float m_averageTime = 0;
	};
}