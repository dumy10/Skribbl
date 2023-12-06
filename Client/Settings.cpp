#include "Settings.h"

Settings::Settings(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.CloseButton, &QPushButton::clicked, this, &Settings::close);
}

//make it a little bit transparent in the future to match the original game settings

Settings::~Settings()
{}
