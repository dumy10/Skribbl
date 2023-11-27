#pragma once

#include <QMainWindow>
#include "ui_OtherPlayerProfile.h"

class OtherPlayerProfile : public QMainWindow
{
	Q_OBJECT

public:
	OtherPlayerProfile(QWidget *parent = nullptr);
	~OtherPlayerProfile();

private:
	Ui::OtherPlayerProfileClass ui;
};
