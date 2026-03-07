#include "ServerConnectForm.h"
#include "utils.h"
#include "RoutingManager.h"

#include <QThread>
#include <regex>

ServerConnectForm::ServerConnectForm(QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.errorLabel->hide();
	connect(m_ui.connectButton, SIGNAL(clicked()), this, SLOT(OnConnectButtonClicked()));
}

void ServerConnectForm::OnConnectButtonClicked()
{
	std::string ip = m_ui.IPField->text().toUtf8().constData();

	try {
		if (ip.empty()) {
			throw std::exception("IP cannot be empty");
		}

		Utils::CheckIpPattern(ip);
		Server::SetIp(ip);

		m_ui.connectButton->setEnabled(false);
		m_ui.errorLabel->hide();

		CheckServerConnectionAsync(Server::GetUrl());
	}
	catch (const std::exception& e) {
		m_ui.connectButton->setEnabled(true);
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(e.what());
	}
}

void ServerConnectForm::CheckServerConnectionAsync(const std::string& url)
{
	QThread* thread = QThread::create([this, url]() {
		bool isRunning = false;
		try {
			isRunning = IsServerRunning(url);
		} catch (...) {
			isRunning = false;
		}

		QMetaObject::invokeMethod(this, "HandleServerConnectionResult", Qt::QueuedConnection, QtPrivate::Invoke::argument<bool>("bool", isRunning)); 
		});

	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
	thread->start();
}

void ServerConnectForm::HandleServerConnectionResult(bool isRunning)
{
	m_ui.connectButton->setEnabled(true);

	if (isRunning) {
		emit NavigateToLogin();
	} else {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText("Server is not running");
	}
}

bool ServerConnectForm::IsServerRunning(const std::string& url) const noexcept
{
	cpr::Response response = RoutingManager::CheckServerRunning(url);

	return Utils::IsResponseSuccessful(response);
}
