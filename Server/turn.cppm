export module turn;

import player;
import <string_view>;
import <chrono>;

namespace skribbl
{
	export class Turn
	{
	public:
		Turn(const Player& player, const std::string& word);

		void setPlayerTurn(const Player& player, const std::string& word);
		void startTimer() noexcept;
		const Player& getCurrentPlayer() const noexcept;
		const std::string_view getCurrentWord() const noexcept;
		const bool guessWord(const std::string& word) const;
	private:
		Player m_player;
		std::string m_word;
		std::chrono::seconds m_drawTime;
		std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	};
}