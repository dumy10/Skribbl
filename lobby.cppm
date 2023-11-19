#include <cstdint>
#include <string>
#include <vector>
export module lobby;
import chat;
import player;

namespace skribbl
{
	export class Lobby {
	public:
		void addUser(const Player& player);
		void removeUser(const Player& player);

	private:
		std::string lobbyLink;
		uint16_t adminId;
		std::vector<Player> lobbyPlayers;
	};
}