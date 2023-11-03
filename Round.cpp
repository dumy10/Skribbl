import round;
using modern::Round;

#include <cstdlib> 
#include <thread>
#include <chrono>
#include<iostream>

void Round::endRound()
{
	std::cout << "Next round start in :";
	for (int i = 5; i > 0; i--)
	{
		std::cout << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
int modern::Round::getRound()
{
	return roundNumber;
}

void modern::Round::modifyRound()
{
	roundNumber++;
}

void clearScreen() {
	system("clear");
}

void Round::startRound() {
	
	if (getRound() == 1)
		std::cout << "Welcome to skribbl \n Your round will begin shortly\n";
	else clearScreen();
	draw();
	modifyRound();
	endRound();
}
