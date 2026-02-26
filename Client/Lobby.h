#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <ui_Lobby.h>
#include <QTimer>

class Lobby : public QWidget
{
	Q_OBJECT

signals:
	void PlayerLeft();
	void NavigateToMenu(const std::string& username);
	void NavigateToGame(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID);

public:
	Lobby(const std::string& username, int playerIndex = 0, bool isOwner = false, const std::string& roomID = "", QWidget* parent = nullptr);
	~Lobby();
	void StopTimer();

private slots:
	void OnCreateLobbyButtonPress();
	void OnStartGameButtonPress();
	void OnBackButtonPress() noexcept;
	void UpdateRoomInformation() noexcept;
	void OnPlayerLeft() const noexcept;
	void CheckGameStarted();

private:
	void GetRoomID();
	void DisplayPlayer(const std::string& username, int index) const noexcept;
	void DisplayPlayerCount(int count) noexcept;
	void DisplayRoomInformation() noexcept;
	void StartTimer();

private:
	Ui::Lobby m_ui;
	std::string m_username;
	std::string m_roomID;
	bool m_isOwner;
	int m_playerIndex;

private:
	std::unique_ptr<QTimer> m_updateTimer;
};
#endif // !LOBBY_H
