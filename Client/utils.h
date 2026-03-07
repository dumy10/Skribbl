#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>
#include <span>

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <cpr/cpr.h>

// See https://stackoverflow.com/a/57346888/12388382
std::vector<std::string> split(const std::string& str, const std::string& delim);
std::unordered_map<std::string, std::string> parseUrlArgs(const std::string& urlArgs);
std::string urlDecode(const std::string& str);

class Server
{
public:
	static void SetIp(const std::string& ip);
	static std::string GetIp();
	static std::string GetUrl();

private:
	static std::string m_ip;
};

class Utils
{
public:
	static void WaitForSeconds(int seconds) noexcept;
	static void CheckPasswordPattern(const std::string& password);
	static void CheckEmailPattern(const std::string& email);
	static void CheckIpPattern(const std::string& ip);
	static void CheckUsernameForRegistration(const std::string& username);
	static void CheckUsernameForLogin(const std::string& username);
	
	// String conversion helpers
	static QString ToQString(std::string_view str) noexcept;
	
	// Response validation helpers
	static [[nodiscard]] bool IsResponseSuccessful(const cpr::Response& response, int expectedCode = 200) noexcept;
	
	// UI helpers
	static void SetWidgetVisibilityByCount(std::span<QWidget*> widgets, int visibleCount) noexcept;
	static void ShowLabelWithText(QLabel* label, std::string_view text) noexcept;
	static void SetLineEditText(QLineEdit* lineEdit, std::string_view text) noexcept;
};