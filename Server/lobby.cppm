export module lobby;

import <string>;
import <vector>;
import chat;
import player;

namespace skribbl
{
	export class Lobby {
	public:
		void addUser(const Player& player);
		void removeUser(const Player& player);
		//more functions for the admins should be added here or in game

	private:
		std::string lobbyLink;
		uint16_t adminId;
		std::vector<Player> lobbyPlayers;
	};
}