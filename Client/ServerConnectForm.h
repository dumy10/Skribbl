#pragma once

#include <QWidget>
#include "ui_ServerConnectForm.h"

class ServerConnectForm : public QWidget
{
	Q_OBJECT

public:
	ServerConnectForm(QWidget* parent = nullptr);

signals:
	void NavigateToLogin();

private slots:
	void OnConnectButtonClicked();
	void HandleServerConnectionResult(bool isRunning);

private:
	void CheckServerConnectionAsync(const std::string& url);
	[[nodiscard]] bool IsServerRunning(const std::string& url) const noexcept;

private:
	Ui::ServerConnectFormClass m_ui;
};
