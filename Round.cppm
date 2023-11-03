export module round;
import <string>;
import "Player.h";
import "Leaderboard.h";
namespace modern {
    export class Round {
    public:
        void startRound();
        void endRound();
        void cleanBoard();
        std::string generateCharacters(const std::string& currentWord);
        Leaderboard updateLeaderboard(const Leaderboard& currentLeaderboard);
        void draw(); //to be implemented, needs GUI
        bool guessWord(std::string word);
        int getRound();
        void modifyRound();
    private:
        Leaderboard currentLeaderboard;
        std::string currentWord;
        uint8_t roundNumber=1;
    };
    
}