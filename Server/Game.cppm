export module game;

import round;
import player;

import <vector>;
import <set>;
import <string>;
import <sstream>;
import <algorithm>;
import <ranges>;

namespace skribbl
{
	export enum class GameStatus : uint8_t
	{
		UNKNOWN,
		WAITING,
		INPROGRESS,
		FINISHED
	};

	export class Game {
	public:
		Game() = default;
		Game(const int id, const Player& player, const std::string& gameCode, const size_t maxPlayers, const size_t currentPlayers);

		[[nodiscard]] const uint8_t GetRoundNumber() const noexcept;

		[[nodiscard]] const int GetId() const noexcept;
		[[nodiscard]] const int GetPlayerIndex(const std::string& username) const noexcept;
		[[nodiscard]] const int GetPlayerScore(const std::string& username) const noexcept;
		[[nodiscard]] const int GetMaxNumberOfRoundsAllowed() const noexcept;
		[[nodiscard]] const int GetGameStatusAsInt() const noexcept;

		[[nodiscard]] const bool GetAllPlayersGuessedWord() const noexcept;

		[[nodiscard]] const size_t GetCurrentPlayerCount() const noexcept;
		[[nodiscard]] const size_t GetNumberOfMaxPlayers() const noexcept;

		[[nodiscard]] const std::string SerializePlayers() const noexcept;
		[[nodiscard]] const std::string SerializePlayerNames() const noexcept;
		[[nodiscard]] const std::string SerializeGameChat() const noexcept;
		[[nodiscard]] const std::string SerializeRound() const noexcept;
		[[nodiscard]] const std::string GetGameCode() const noexcept;
		[[nodiscard]] const std::string GetDrawingPlayer() const noexcept;

		[[nodiscard]] const GameStatus GetGameStatus() const noexcept;

		inline [[nodiscard]] Round& GetRound() noexcept { return m_round; }

		[[nodiscard]] const std::vector<Player> GetPlayers() const noexcept;
		[[nodiscard]] const std::vector<std::string> GetChatLines() const noexcept;

		[[nodiscard]] bool StartGame(const std::set<std::string>& words) noexcept;
		[[nodiscard]] bool EndGame() noexcept;
		[[nodiscard]] bool NextRound() noexcept;
		[[nodiscard]] bool AddChatLineMessage(const std::string& message, const std::string& username, bool& guessed) noexcept;


		void AddPlayer(const Player& player) noexcept;
		void RemovePlayer(const Player& player) noexcept;
		void SetGameCode(const std::string& gameCode) noexcept;
		void SetGameStatus(const GameStatus status) noexcept;
		void SetGameStatusFromInt(int status) noexcept;
		void SetMaxPlayers(size_t maxPlayers) noexcept;
		void SetId(int id) noexcept;
		void SetPlayers(const std::vector<Player>& players) noexcept;
		void DeserializePlayers(const std::string& serializedPlayers) noexcept;
		void DeserializeGameChat(const std::string& serializedChatLines) noexcept;
		void DeserializeRound(const std::string& serializedRound) noexcept;

		Game(const Game& other) = delete;
		Game& operator=(const Game& other) = delete;

		// Move constructor and move assignment operator
		Game(Game&& other);
		Game& operator=(Game&& other);

	public:
		[[nodiscard]] bool operator==(const Game& other) const noexcept;

	private:
		int m_id;
		size_t m_currentPlayers;
		size_t m_maxPlayers;

		std::string m_gameCode;

		std::vector<std::string> m_chatLineMessages;
		std::vector<Player> m_players;

		Round m_round;
		GameStatus m_gameStatus{ GameStatus::UNKNOWN };

	private:
		static constexpr size_t kMaxNumberOfRounds{ 4 };
	};
}