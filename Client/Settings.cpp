#include "Settings.h"

Settings::Settings(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	connect(m_ui.CloseButton, &QPushButton::clicked, this, &Settings::close);
}

Settings::~Settings()
{}
