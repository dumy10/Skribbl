#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <QWidget>

#include "ui_Lobby.h"

class Lobby : public QMainWindow
{
	Q_OBJECT

public:
	Lobby(QWidget *parent = nullptr);
	~Lobby();
	QPlainTextEdit* textEdit;
	void enterEvent(QEvent* event);

private:
	Ui::LobbyClass m_ui;

};
