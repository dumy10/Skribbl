module game; 
import turn;
import <iostream>;
import <string>;
import <fstream>;

using namespace skribbl;




void Game::addPlayer(const Player& player)
{
	this->m_players.push_back(player);
}

void Game::removePlayer(const Player& player)
{
	for(size_t i = 0; i < this->m_players.size(); i++)
	{
		if(this->m_players[i] == player)
		{
			this->m_players.erase(this->m_players.begin() + i);
			break;
		}
	}
}

/*
startGame function needs rethinking
*/

void Game::startGame()
{
	std::string password, username;
	std::cout << "Welcome to skribbl";
	std::cout << "Login";//make this a button in the future
	//the first player in the game should become the admin and be able to make the others admin if he wants to
	std::cin >> username >> password;
	//search the username in the database and see if the password is correct
	//primul jucator care intra este facut admin
	while (m_players.size() < 2) //do this every 10 secconds
	{
		std::cout << "Waiting for more players";
		std::cout << "The invite link is:"; //put the link to the game
	}
	std::cout << "The game will start shortly";
	/* Cannot cast a nonstatic member function to a function pointer
	while (Round::getRound() < 5) //the admin will be able to modify it
	{
		//Round::startRound();
		//Round::modifyRound();
	}*/
	endGame();
}

void Game::endGame()
{
	//clearScreen(); function not declared
	std::cout << "The winner is : "; //show the first place <<
	std::cout << "Points : ";//show the points 
	std::cout << "\n Seccond place : ";
	std::cout << "Points : ";
	std::cout << "\n Third place : ";
	std::cout << "Points:";
	std::cout << "\n Play again?";
	std::cout << "Leave";//make them buttons in the future
}

/* Function not defined properly
void clearScreen()
{
	system("clear");
}*/

Player Game::getWinner(const Leaderboard& leaderboard) const
{
	if(leaderboard.getLeaderboard().size() == 0)
		return Player();
	else
		return leaderboard.getLeaderboard()[0];
}

void skribbl::Game::makeAdmin(const Player& player)
{
	for (size_t i = 0; i < this->m_players.size(); i++)
	{
		if (this->m_players[i] == player)
		{
			this->m_players[i].makeAdmin();
			break;
		}
	}
}

void skribbl::Game::removeAdmin(const Player& player)
{
	for (size_t i = 0; i < this->m_players.size(); i++)
	{
		if (this->m_players[i] == player)
		{
			this->m_players[i].removeAdmin();
			break;
		}
	}
}
