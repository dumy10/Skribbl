#pragma once

#include <QMainWindow>
#include "ui_Menu.h"

class Menu : public QMainWindow
{
	Q_OBJECT

public:
	Menu(const std::string& username, QWidget* parent = nullptr);
	~Menu() = default;

private:
	Ui::MenuClass m_ui;
	std::string m_username;

private:
	void WaitForSeconds(int seconds) const noexcept;

private slots:
	void OnCreateButtonClicked() noexcept;
	void OnJoinButtonClicked() noexcept;
	void OnJoinGameButtonClicked();
};
