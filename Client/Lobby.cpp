#include "Lobby.h"

Lobby::Lobby(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	m_ui.label_2->setText("Linkul pentru invite");
	connect(m_ui.pushButton_3 ,  SIGNAL(clicked()), this, SLOT(onpushButton_3()));
}

Lobby::~Lobby()
{}

void Lobby::enterEvent(QEvent * event)
{
	Q_UNUSED(event);
	m_ui.label_2->hide(); //sa se ascunda label_2 cand ajunge clickul peste el
}
