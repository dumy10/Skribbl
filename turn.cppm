export module turn;

import player;
import <string_view>;


namespace skribbl
{
	export class Turn
	{
	public:
		Turn(const Player& player, const std::string& word);
		void setPlayerTurn(const Player& player, const std::string& word);
		const Player& getCurrentPlayer() const noexcept;
		const std::string_view getCurrentWord() const noexcept;
		const bool guessWord(const std::string& word) const;
	private:
		Player m_player;
		std::string m_word;
	};
}