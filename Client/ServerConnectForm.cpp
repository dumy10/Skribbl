#include "ServerConnectForm.h"
#include "utils.h"
#include "RoutingManager.h"

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

	try
	{
		if (ip.empty()) {
			throw std::exception("IP cannot be empty");
		}

		CheckIp(ip);
		Server::SetIp(ip);

		if (!IsServerRunning(Server::GetUrl())) {
			throw std::exception("Server is not running");
		}

		emit NavigateToLogin();
	}
	catch (const std::exception& e)
	{
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(e.what());
	}
}

void ServerConnectForm::CheckIp(const std::string& ip) const
{
	std::regex ipv4("((0?[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}(0?[0-9]|[1-9][0-9]?|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
	if (!std::regex_match(ip, ipv4)) {
		throw std::exception("Invalid IP address");
	}
}

bool ServerConnectForm::IsServerRunning(const std::string& url) const noexcept
{
	cpr::Response response = RoutingManager::CheckServerRunning(url);

	return response.status_code == 200;
}
