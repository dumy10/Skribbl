export module round;

import player;
import turn;

import <vector>;
import <string>;


namespace skribbl
{
	export class Round {
	public:

		Round(const Turn& turn, const std::string& currentWord, uint8_t roundNumber, const std::vector<Player>& players);

		void StartRound();
		void EndRound();
		void SetWord(const std::string& word);

		const bool GuessWord(const std::string& word) const;
		const int GetRound() const noexcept;
		const std::string GetWord() const noexcept;

		void ModifyRound();
		void SetTurn(const Player& player, const std::string& word);
		std::string GetDrawer();

	private:
		// Use a reference to the player instead of copying it
		std::vector<Player> m_players;
		Turn m_turn;
		std::string m_word; // turn should have the word , not round
		uint8_t m_roundNumber;
	};

}