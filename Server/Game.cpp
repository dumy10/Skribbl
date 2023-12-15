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
