import game;
import player;
import <iostream>;
import <vector>;



using namespace skribbl;



int main()
{
	Player p1("Player1", "pass1", "p1@gmail.com");
	Player p2("Player2", "pass2", "p2@gmail.com");
	Player p3("Player3", "pass3", "p3@gmail.com");
	std::vector<Player> playersList;
	playersList.push_back(p1);
	playersList.push_back(p2);
	playersList.push_back(p3);

	Game game(playersList);
	
	return 0;
}