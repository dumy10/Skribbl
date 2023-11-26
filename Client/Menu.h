#pragma once

#include <QMainWindow>
#include "ui_Menu.h"

class Menu : public QMainWindow
{
	Q_OBJECT

public:
	Menu(QWidget *parent = nullptr);
	~Menu();
private:
	Ui::MenuClass ui;
};
