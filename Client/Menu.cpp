#include "Menu.h"
#include "Lobby.h"
#include <QPainter>
Menu::Menu(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
connect(m_ui.createRoom, SIGNAL(clicked()), this, SLOT(onCreateButtonClicked()));
}

Menu::~Menu()
{}

void Menu::onCreateButtonClicked()
{
	// open new lobby
	Lobby* lobby = new Lobby();
	lobby->show();
	this->close();
}

void Menu::onJoinButtonClicked()
{
}
