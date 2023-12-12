#pragma once

#include <unordered_map>
#include  <string>
#include  <vector>

// See https://stackoverflow.com/a/57346888/12388382
std::vector<std::string> split(const std::string& str, const std::string& delim);

std::unordered_map<std::string, std::string> parseUrlArgs(const std::string& urlArgs);

class Server
{
public:
	static void SetIp(const std::string& ip);
	static std::string GetIp();
	static std::string GetUrl();

private:
	static std::string m_ip;
};