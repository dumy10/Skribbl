export module chat;

import player;
import <vector>;
import <iostream>;
export import <string>;

namespace skribbl {
    export class Chat {
    public:
        void isMatch(); // player-ul a gasit cuvantul
        void addListener(const Player& player);
        void removeListener(const Player& player);
        void Notify();  // player-ul a fost afk un anumit timp
        void SendMessage(const Player& player,std::string message);


    private:
        uint16_t characterLimit;
        uint8_t spamLimit;
        std::vector<Player> listeners;
    };
}
