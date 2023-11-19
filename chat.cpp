import chat;
import player;
import <iostream>;
using namespace skribbl;

void Chat::isMatch()
{
	//Logica pentru cand un jucator a gasit un cuvant
}

void Chat::addListener(const Player& player)
{
	listeners.push_back(player);
}

void Chat::removeListener(const Player& player)
{
	auto it = std::find(listeners.begin(), listeners.end(), player);
	if (it != listeners.end())
		listeners.erase(it);
}

void Chat::Notify()
{
	//Logica pentru notificarea ascultatorilor (de exemplu daca un jucator a fost AFK)
	for (const auto& listener : listeners)
	{
		//Notificare
	}
}

void Chat::SendMessage(const Player& player,std::string message)
{
	std::cout << player.getName() << ":" << message << "\n";
}