#include "Menu.h"
#include "Lobby.h"
#include <QPainter>

Menu::Menu(const std::string& username, QWidget* parent)
	: QMainWindow(parent),
	m_username(username)
{
	m_ui.setupUi(this);
	connect(m_ui.createRoom, SIGNAL(clicked()), this, SLOT(onCreateButtonClicked()));
}

//consructor of menu with string parameters, to be used with move semantics


Menu::~Menu()
{}

void Menu::onCreateButtonClicked()
{
	// open new lobby
	Lobby* lobby = new Lobby(std::move(m_username));
	lobby->show();
	this->close();
}

void Menu::onJoinButtonClicked()
{
}
