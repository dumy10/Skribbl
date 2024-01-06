export module round;

import player;

import <vector>;
import <string>;
import <set>;
import <chrono>;
import <thread>;

namespace skribbl
{
	export class Round {
	public:

		Round(uint8_t roundNumber,const std::vector<Player>& players,int playerDrawing, std::set<std::string> words);

		void StartRound();
		void EndRound();
		void SetWords(const std::set<std::string>& words);

		const bool GuessWord(const std::string& word) const;
		const int GetRound() const noexcept;
		const std::set<std::string> GetWords() const noexcept;

		void ModifyRound();

	private:
		// Use a reference to the player instead of copying it
		int m_playerDrawing;
		std::vector<Player> m_players;
		std::set<std::string> m_words; // turn should have the word , not round
		uint8_t m_roundNumber; //game should have it
	};

}