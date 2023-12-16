export module turn;

import player;
import <chrono>;
import <string>;

namespace skribbl
{
	export class Turn
	{
	public:
		Turn(const Player& player, const std::string& word);

		void SetPlayerTurn(const Player& player, const std::string& word);
		void StartTimer() noexcept;

		const Player& GetCurrentPlayer() const noexcept;
		const std::string GetCurrentWord() const noexcept;
		const bool GuessWord(const std::string& word) const;

	private:
		// Use a reference to the player instead of copying it
		Player m_player;
		std::string m_word;
		std::chrono::seconds m_drawTime;
		std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	};
}