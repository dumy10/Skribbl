#pragma once

#include <QWidget>
#include "ui_ServerConnectForm.h"

class ServerConnectForm : public QWidget
{
	Q_OBJECT

signals:
	void NavigateToLogin();

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
