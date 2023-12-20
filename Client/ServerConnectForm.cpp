#include "ServerConnectForm.h"
#include "LoginForm.h"
#include "utils.h"

#include <regex>
#include <cpr/cpr.h>
#include <crow.h>

ServerConnectForm::ServerConnectForm(QWidget *parent)
	: QMainWindow(parent)
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
		if(ip == "")
			throw std::exception("IP cannot be empty");

		CheckIp(ip);
		Server::SetIp(ip);

		if (!IsServerRunning(Server::GetUrl()))
			throw std::exception("Server is not running");

		LoginForm* loginForm = new LoginForm();
		loginForm->show();

		this->close();
	}
	catch (const std::exception& e)
	{
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(e.what());
	}
}

void ServerConnectForm::CheckIp(const std::string& ip)
{
	std::regex ipv4("((0?[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}(0?[0-9]|[1-9][0-9]?|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
	if (!std::regex_match(ip, ipv4))
		throw std::exception("Invalid IP address");
}

bool ServerConnectForm::IsServerRunning(const std::string& url)
{
	cpr::Response response = cpr::Get(
		cpr::Url{ url + "/"}
	);

	return response.status_code == 200;
}
