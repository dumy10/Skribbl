#pragma once

#include <QMainWindow>
#include "ui_PlayerProfile.h"

class PlayerProfile : public QMainWindow
{
	Q_OBJECT

public:
	PlayerProfile(QWidget *parent = nullptr);
	~PlayerProfile();

private:
	Ui::PlayerProfileClass m_ui;
};
