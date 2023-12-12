#include "OtherPlayerProfile.h"

OtherPlayerProfile::OtherPlayerProfile(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
}

//make it a little bit transparent in the future to match the original game settings
//the report button should send a message to the admin with the player problem

OtherPlayerProfile::~OtherPlayerProfile()
{}
