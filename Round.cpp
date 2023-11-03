import round;
using modern::Round;
#include <cstdlib> 
#include <thread>
#include <chrono>
#include<iostream>

void Round::endRound()
{
	std::cout << "Next round start in :";
	for (int i = 1; i < 6; i++)
	{
		std::cout << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
void Round::startRound()
{

}

void clearScreen() {
	system("clear");
}