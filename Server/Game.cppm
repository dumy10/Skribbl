export module game;

import round;
import player;
import <vector>;
import <string>;
import <array>;
import <sstream>;


namespace skribbl
{
	export class Game {
	public:
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
		std::string GetGameCode() const noexcept;
		const GameStatus GetGameStatus() const noexcept;
		const size_t GetMaxPlayers() const noexcept;
		const std::vector<Player>& GetPlayers() const noexcept;
		int GetId() const noexcept;

		void SetGameCode(const std::string& gameCode);
		void SetGameStatus(GameStatus status);
		void SetMaxPlayers(size_t maxPlayers);
		void SetCurrentPlayers(int currentPlayers);
		void SetId(int id);
		void SetPlayers(const std::vector<Player>& players);
		void SetGameStatusInt(int status);
		//const Player& getWinner() const;

	private:
		int m_id;
		static const size_t kNoOfRounds{ 4 };
		size_t m_maxPlayers;
		std::string m_gameCode;
		std::vector<Player> m_players; // map with players and their scores to iterate more easily in order to change the scores / check winner / check if player is in game??
		std::vector<Round> m_rounds; // is a vector needed? maybe a single round is enough and change/modify round after each round ends
		GameStatus m_gameStatus{ GameStatus::UNKNOWN };
	};
}