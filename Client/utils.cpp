#include "utils.h"
#include "RoutingManager.h"

#include <sstream>
#include <regex>
#include <QTime>
#include <QCoreApplication>

// See https://stackoverflow.com/a/57346888/12388382
std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> result;
	size_t startIndex = 0;

	for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex)) {
		result.emplace_back(str.begin() + startIndex, str.begin() + found);
		startIndex = found + delim.size();
	}

	if (startIndex != str.size()) {
		result.emplace_back(str.begin() + startIndex, str.end());
	}

	return result;
}

std::unordered_map<std::string, std::string> parseUrlArgs(const std::string& urlArgs)
{
	if (urlArgs.empty()) {
		return {};
	}

	std::unordered_map<std::string, std::string> params;
	std::istringstream stream{ urlArgs };
	std::string pair;

	while (std::getline(stream, pair, '&')) {
		size_t equalsPos = pair.find('=');
		if (equalsPos != std::string::npos) {
			std::string key = urlDecode(pair.substr(0, equalsPos));
			std::string value = urlDecode(pair.substr(equalsPos + 1));
			params[key] = value;
		}
	}

	return params;
}

std::string urlDecode(const std::string& str)
{
	std::string result;
	result.reserve(str.size());

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '%' && i + 2 < str.size()) {
			// Convert hex to char
			int hexValue;
			std::istringstream hex_stream{ str.substr(i + 1, 2) };
			if (hex_stream >> std::hex >> hexValue) {
				result += static_cast<char>(hexValue);
				i += 2;
			}
			else {
				result += str[i];
			}
		}
		else if (str[i] == '+') {
			result += ' '; // '+' represents space in form data
		}
		else {
			result += str[i];
		}
	}

	return result;
}

std::string Server::m_ip;

void Server::SetIp(const std::string& ip)
{
    m_ip = ip;
}

std::string Server::GetIp()
{
    return m_ip;
}

std::string Server::GetUrl()
{
    return "http://" + m_ip + ":18080";
}

void Utils::WaitForSeconds(int seconds) noexcept
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

void Utils::CheckPasswordPattern(const std::string& password)
{
	if (password.empty()) {
		throw std::exception("Password cannot be empty");
	}

	if (password.length() < 6) {
		throw std::exception("Password must be at least 6 characters long");
	}

	const std::regex passwordPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{6,}$");
	if (!std::regex_match(password, passwordPattern)) {
		throw std::exception("The Password needs to contain at least 1 upper case,\n one lower case and a number");
	}
}

void Utils::CheckEmailPattern(const std::string& email)
{
	if (email.empty()) {
		throw std::exception("Email cannot be empty");
	}

	const std::regex emailPattern("^([a-zA-Z0-9_\\.\\-+]+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,5})$");

	if (!std::regex_match(email, emailPattern)) {
		throw std::exception("Email should be like: example@mail.com");
	}
}

void Utils::CheckUsernameForRegistration(const std::string& username)
{
	if (username.empty()) {
		throw std::exception("Username cannot be empty");
	}

	cpr::Response response = RoutingManager::CheckUsername(username);

	if (response.status_code != 200 && response.status_code != 404) {
		throw std::exception("Server error");
	}

	if (response.text == "EXISTING") {
		throw std::exception("Username already taken");
	}
}

void Utils::CheckUsernameForLogin(const std::string& username)
{
	if (username.empty()) {
		throw std::exception("Username cannot be empty");
	}

	cpr::Response response = RoutingManager::CheckUsername(username);

	if (response.status_code != 200) {
		throw std::exception("Username or password are invalid.");
	}
}