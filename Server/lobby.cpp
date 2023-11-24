module lobby;

import chat; 
import <vector>;
import <algorithm>;
import <iostream>;
import <string>;
import player;  

namespace skribbl {

    void Lobby::addUser(const Player& player) {
        auto it = std::find(lobbyPlayers.begin(), lobbyPlayers.end(), player);
        if (it == lobbyPlayers.end()) {
            lobbyPlayers.push_back(player);
        }
        else {
            std::cout << "Jucatorul este deja in lobby." << std::endl;
        }
    }

    void Lobby::removeUser(const Player& player) {
        auto it = std::find(lobbyPlayers.begin(), lobbyPlayers.end(), player);
        if (it != lobbyPlayers.end()) {
            lobbyPlayers.erase(it);
        }
        else {
            std::cout << "Jucatorul nu este in lobby." << std::endl;
        }
    }
}
