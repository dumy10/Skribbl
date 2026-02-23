export module round;

import timer;

import <vector>;
import <set>;
import <string>;
import <sstream>;
import <algorithm>;
import <ranges>;

namespace skribbl
{
	export class Round {
	public:

		Round() = default;
		Round(const std::string& gameId, const std::set<std::string>& words, const size_t maxPlayers);

		[[nodiscard]] bool StartRound(const std::string& drawingPlayerName, const uint8_t roundNumber) noexcept;
		[[nodiscard]] bool StopRound() noexcept;

		void SetRoundNumber(uint8_t roundNumber) noexcept;
		void SetGameId(const std::string& gameId) noexcept;
		void SetDrawingPlayer(const std::string& drawingPlayerName) noexcept;
		void SetCurrentWord(const std::string& currentWord) noexcept;
		void SetImageData(const std::string& imageData) noexcept;
		void SetTimes(const std::vector<int>& times) noexcept;
		void SetWords(const std::set<std::string>& words) noexcept;
		void PlayerGuessedWord(const std::string& playerName, const int index, const int timeLeft) noexcept;

		void DeserializeTimes(const std::string& serializedPoints) noexcept;
		void DeserializeWords(const std::string& serializedWords) noexcept;
		void Deserialize(const std::string& serializedRound);
		void UpdateTimes(const int index, const int value) noexcept;

		[[nodiscard]] const int GetTimeLeft() const noexcept;
		[[nodiscard]] const int CalculatePoints(const int time) const noexcept;

		[[nodiscard]] const uint8_t GetRoundNumber() const noexcept;

		[[nodiscard]] const std::string GetGameId() const noexcept;
		[[nodiscard]] const std::string GetDrawingPlayer() const noexcept;
		[[nodiscard]] const std::string GetCurrentWord() const noexcept;
		[[nodiscard]] const std::string SerializeWords() const noexcept;
		[[nodiscard]] const std::string SerializeTimes() const noexcept;
		[[nodiscard]] const std::string Serialize() const noexcept;
		[[nodiscard]] const std::string GetImageData() const noexcept;

		[[nodiscard]] const std::vector<int> GetTimes() const noexcept;
		[[nodiscard]] const std::vector<std::string> GetGuessedPlayerNames() const noexcept;
		[[nodiscard]] const std::set<std::string> GetWords() const noexcept;

		Round(const Round& other) = delete;
		Round& operator=(const Round& other) = delete;

		// Move constructor and move assignment operator
		Round(Round&& other) noexcept;
		Round& operator=(Round&& other) noexcept;

	private:
		uint8_t m_roundNumber;

		std::string m_gameId;
		std::string m_drawingPlayerName;
		std::string m_currentWord;
		std::string m_imageData;

		std::vector<int> m_times;
		std::vector<std::string> m_guessedPlayerNames;
		std::set<std::string> m_words;

		Timer m_timer;
	};
}