export module round;

import player;
import leaderboard;
import player;
import turn;

import <vector>;
import <string>;


namespace skribbl 
{
    export class Round {
    public:

        Round(const Leaderboard& leaderboard, const Turn& turn, const std::string& currentWord, uint8_t roundNumber, const std::vector<Player>& players);
        void startRound();
        void endRound();
        void setWord(const std::string& word);
        void setLeaderboard(const std::vector<Player>& players);
        void draw(); //to be implemented, needs GUI
        const bool guessWord(const std::string& word) const;
        const int getRound() const noexcept;
        void modifyRound();
        void setTurn(const Player& player); 

    private:
        std::vector<Player> m_players;
        Leaderboard m_leaderboard;
        Turn m_turn;
        std::string m_word;
		uint8_t m_roundNumber;
    };
    
}