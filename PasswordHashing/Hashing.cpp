#include "Hashing.h"
#include <windows.h>
#include <wincrypt.h>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

#pragma comment(lib, "advapi32.lib")

void Hasher::GenerateSalt(char* outBuffer, size_t bufferSize, size_t saltLength)
{
	if (!outBuffer || bufferSize < (saltLength * 2 + 1)) {
		return; // Not enough space for hex string + null terminator
	}
	
	std::vector<unsigned char> salt(saltLength);
	HCRYPTPROV hProv;
	
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		// Fallback to non-cryptographic random if CryptoAPI fails
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);
		
		for (size_t i = 0; i < saltLength; ++i) {
			salt[i] = static_cast<unsigned char>(dis(gen));
		}
	}
	else {
		CryptGenRandom(hProv, static_cast<DWORD>(saltLength), salt.data());
		CryptReleaseContext(hProv, 0);
	}
	
	std::string hexString = ToHexString(salt);
	strncpy_s(outBuffer, bufferSize, hexString.c_str(), _TRUNCATE);
}

void Hasher::HashPasswordWithSalt(const char* password, const char* salt, char* outBuffer, size_t bufferSize)
{
	if (!password || !salt || !outBuffer || bufferSize < 65) {
		return; // Need at least 65 bytes for SHA-256 hex (64) + null terminator
	}
	
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	std::vector<unsigned char> hash(32); // SHA-256 produces 32 bytes
	DWORD hashLen = 32;
	
	try {
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
			outBuffer[0] = '\0';
			return;
		}
		
		if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
			CryptReleaseContext(hProv, 0);
			outBuffer[0] = '\0';
			return;
		}
		
		// Hash password
		if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(password), 
			static_cast<DWORD>(std::strlen(password)), 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			outBuffer[0] = '\0';
			return;
		}
		
		// Hash salt
		if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(salt), 
			static_cast<DWORD>(std::strlen(salt)), 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			outBuffer[0] = '\0';
			return;
		}
		
		if (!CryptGetHashParam(hHash, HP_HASHVAL, hash.data(), &hashLen, 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			outBuffer[0] = '\0';
			return;
		}
		
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		
		std::string hexString = ToHexString(hash);
		strncpy_s(outBuffer, bufferSize, hexString.c_str(), _TRUNCATE);
	}
	catch (...) {
		if (hHash) CryptDestroyHash(hHash);
		if (hProv) CryptReleaseContext(hProv, 0);
		outBuffer[0] = '\0';
	}
}

bool Hasher::VerifyPassword(const char* password, const char* salt, const char* hash)
{
	if (!password || !salt || !hash) {
		return false;
	}
	
	char computedHash[65]; // 64 hex chars + null terminator
	HashPasswordWithSalt(password, salt, computedHash, sizeof(computedHash));
	
	return std::strcmp(computedHash, hash) == 0;
}

std::string Hasher::ToHexString(const std::vector<unsigned char>& data)
{
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	
	for (unsigned char byte : data) {
		oss << std::setw(2) << static_cast<int>(byte);
	}
	
	return oss.str();
}

std::vector<unsigned char> Hasher::FromHexString(const std::string& hex)
{
	std::vector<unsigned char> result;
	
	for (size_t i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
		result.push_back(byte);
	}
	
	return result;
}
