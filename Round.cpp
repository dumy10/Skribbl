module round;
import leaderboard;
import player;
import turn;
import wordgenerator;

using namespace skribbl;

import <cstdlib>;
import <thread>;
import <chrono>;
import <iostream>;

Round::Round(const Leaderboard& leaderboard, const Turn& turn, const std::string& currentWord, uint8_t roundNumber, const std::vector<Player>& players):
	m_leaderboard{leaderboard},
	m_turn{turn},
	m_word{currentWord},
	m_roundNumber{roundNumber},
	m_players{players}
{
	// Empty
}

void Round::endRound()
{
	/*
	End round:
		- Display leaderboard
		- Display word
		- Display next round start in 5 seconds
	*/
	std::cout << "Next round start in :";
	for (int i = 5; i > 0; i--)
	{
		std::cout << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

const int skribbl::Round::getRound() const noexcept
{
	return m_roundNumber;
}

void skribbl::Round::modifyRound()
{
	/*
	Next round:
		- Clear board
		- Set new word
		- Set new turn
		- Set new leaderboard
	*/


	m_roundNumber++;
}

void clearScreen() 
{
	system("clear");
}

void Round::startRound() 
{
	/*
	Start round:
		- Display welcome message
		- Display word to guess censored
		- Display leaderboard
		- Display who's turn is
		- Display timer
	*/

	if (getRound() == 1)
		std::cout << "Welcome to skribbl \n Your round will begin shortly\n";
	else 
	{
		clearScreen();
		//draw();  //to be implemented later, leave it comented for now	as it will cause compilation errors
		modifyRound();
		endRound();
	}
}

void Round::setLeaderboard(const std::vector<Player>& players)
{
	this->m_leaderboard.setLeaderboard(players);
}

void Round::setWord(const std::string& word)
{
	this->m_word = word;
}


const bool Round::guessWord(const std::string& word) const
{
	return word == m_word;
}

void Round::setTurn(const Player& player)
{
	this->m_turn.setPlayerTurn(player, WordGenerator::generateWord());
}