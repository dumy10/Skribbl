#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <ui_Lobby.h>
#include <QTimer>

class Lobby : public QMainWindow
{
	Q_OBJECT

signals:
	void PlayerLeft();
public:
	Lobby(const std::string& username, int playerIndex = 0, bool isOwner = false, const std::string& roomID = "", QWidget* parent = nullptr);
	~Lobby();

private slots:
	void OnCreateLobbyButtonPress();
	void OnStartGameButtonPress();
	void OnBackButtonPress();
	void UpdateRoomInformation();
	void OnPlayerLeft();
	void CheckGameStarted();

private:
	void GetRoomID();
	void DisplayPlayer(const std::string& username, int index);
	void DisplayPlayerCount(int count);
	void DisplayRoomInformation();
	void WaitForSeconds(int seconds);
	void StartTimer();
	void closeEvent(QCloseEvent* event) override;

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
