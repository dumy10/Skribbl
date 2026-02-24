#pragma once

#include <QWidget>
#include "ui_Menu.h"

class Menu : public QWidget
{
	Q_OBJECT

signals:
	void NavigateToLobby(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID);

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
