#include "Settings.h"

Settings::Settings(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.CloseButton, &QPushButton::clicked, this, &Settings::close);
}

Settings::~Settings()
{}
