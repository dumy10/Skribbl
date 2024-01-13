export module game;

import round;
import player;
import <vector>;
import <string>;
import <sstream>;

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
		int GetPlayerScore(const std::string& username) const noexcept;
		int GetNoOfRounds() const noexcept;
		std::string GetChat() const noexcept;
		std::string GetDrawingPlayer() const noexcept;
		uint8_t GetRoundNumber() const noexcept;
		int GetPlayerIndex(const std::string& username) const noexcept;

		void SetGameCode(const std::string& gameCode);
		void SetGameStatus(GameStatus status);
		void SetMaxPlayers(size_t maxPlayers);
		void SetCurrentPlayers(int currentPlayers);
		void SetId(int id);
		void SetPlayers(const std::vector<Player>& players);
		void SetGameStatusInt(int status);
		void SetChat(const std::string& chat);

	private:
		int m_id;
		static const size_t kNoOfRounds{ 4 };
		size_t m_currentPlayers;
		size_t m_maxPlayers;
		std::string m_chat;
		std::string m_gameCode;
		std::vector<Player> m_players;
		Round m_round;
		GameStatus m_gameStatus{ GameStatus::UNKNOWN };
	};
}