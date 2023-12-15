export module round;

import player;
import player;
import turn;

import <vector>;
import <string>;


namespace skribbl 
{
    export class Round {
    public:

        Round(const Turn& turn, const std::string& currentWord, uint8_t roundNumber, const std::vector<Player>& players);
        void startRound();
        void endRound();
        void setWord(const std::string& word);
        const bool guessWord(const std::string& word) const;
        const int getRound() const noexcept;
        void modifyRound();
        void setTurn(const Player& player); 

    private:
        std::vector<Player> m_players;
        Turn m_turn;
        std::string m_word;
		uint8_t m_roundNumber;
    };
    
}