#pragma once

#include <QMainWindow>
#include "ui_ServerConnectForm.h"

class ServerConnectForm : public QMainWindow
{
	Q_OBJECT

public:
	ServerConnectForm(QWidget* parent = nullptr);

private:
	void CheckIp(const std::string& ip) const;
	[[nodiscard]] bool IsServerRunning(const std::string& url) const noexcept;
private:
	Ui::ServerConnectFormClass m_ui;

private slots:
	void OnConnectButtonClicked();
};
