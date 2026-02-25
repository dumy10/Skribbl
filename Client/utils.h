#pragma once

#include <unordered_map>
#include <string>
#include <vector>

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
	static void CheckUsernameForRegistration(const std::string& username);
	static void CheckUsernameForLogin(const std::string& username);
};