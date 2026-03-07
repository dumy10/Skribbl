#pragma once
#include "LibDefine.h"
#include <string>
#include <vector>
#include <cstring>

class DLL_API Hasher
{
public:
	Hasher() = delete;
	Hasher(const Hasher&) = delete;
	
	// Secure hashing methods using char buffers for DLL safety
	// GenerateSalt: Generates a random salt and writes it to outBuffer
	// bufferSize must be at least (saltLength * 2 + 1) for hex string + null terminator
	static void GenerateSalt(char* outBuffer, size_t bufferSize, size_t saltLength = 32);
	
	// HashPasswordWithSalt: Hashes password with salt and writes result to outBuffer
	// bufferSize must be at least 65 bytes (64 hex chars + null terminator) for SHA-256
	static void HashPasswordWithSalt(const char* password, const char* salt, char* outBuffer, size_t bufferSize);
	
	// VerifyPassword: Verifies if password matches the hash with given salt
	[[nodiscard]] static bool VerifyPassword(const char* password, const char* salt, const char* hash);
	
private:
	[[nodiscard]] static std::string ToHexString(const std::vector<unsigned char>& data);
	[[nodiscard]] static std::vector<unsigned char> FromHexString(const std::string& hex);
};