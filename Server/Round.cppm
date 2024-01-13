export module round;

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
		Round(int id, const std::string& gameId, size_t maxPlayers);

		void SetId(int id);
		void SetGameId(std::string gameId);
		void SetDrawingPlayer(const std::string& drawingPlayerName);
		void SetCurrentWord(const std::string& currentWord);
		void SetWords(const std::set<std::string>& words);
		void SetRoundNumber(uint8_t roundNumber);
		void DeserializeTimes(const std::string& serializedPoints);
		void SetTimes(const std::vector<int>& times);
		void SetImageData(const std::string& imageData);

		int GetId() const noexcept;
		std::string GetGameId() const noexcept;
		std::string GetDrawingPlayer() const noexcept;
		std::string GetCurrentWord() const noexcept;
		uint8_t GetRoundNumber() const noexcept;
		std::string SerializeWords() const noexcept;
		void DeserializeWords(const std::string& serializedWords);
		std::set<std::string> GetWords() const noexcept;
		std::string SerializeTimes() const noexcept;
		std::vector<int> GetTimes() const noexcept;
		void UpdateTimes(int index, int value) noexcept;
		std::string GetImageData() const noexcept;

		void SetTimeLeft(int timeLeft);
		int GetTimeLeft() const noexcept;

	private:
		int m_id;
		std::string m_gameId;
		std::string m_drawingPlayerName;
		std::string m_currentWord;
		std::string m_imageData;
		std::set<std::string> m_words;
		std::vector<int> m_times;
		uint8_t m_roundNumber;
		int m_timeLeft;
	};
}