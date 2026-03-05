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

	try {
		if (ip.empty()) {
			throw std::exception("IP cannot be empty");
		}

		Utils::CheckIpPattern(ip);
		Server::SetIp(ip);

		if (!IsServerRunning(Server::GetUrl())) {
			throw std::exception("Server is not running");
		}

		emit NavigateToLogin();
	} catch (const std::exception& e) {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(e.what());
	}
}

bool ServerConnectForm::IsServerRunning(const std::string& url) const noexcept
{
	cpr::Response response = RoutingManager::CheckServerRunning(url);

	return Utils::IsResponseSuccessful(response);
}
