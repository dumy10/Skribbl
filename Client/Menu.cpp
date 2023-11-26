#include "Menu.h"
#include <QPainter>
Menu::Menu(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

Menu::~Menu()
{}

void Menu::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	QPixmap background("D:\Namenotfound1.git\Client\skribbl.png");
	painter.drawPixmap(0, 0, width(), height(), background);
}
