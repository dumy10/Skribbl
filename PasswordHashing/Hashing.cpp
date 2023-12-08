#include "Hashing.h"

std::string Hasher::HashPassword(const std::string& password)
{
	std::string hashedPassword;
	char hashedChar;
	for (const char& c : password) {
		if (isalpha(c)) {
			char base = (isupper(c)) ? 'A' : 'a';
			hashedChar = base + (c - base + 3) % 26;
			hashedPassword += hashedChar;
		}
		else if (isdigit(c)) {
			hashedChar = '0' + (c - '0' + 3) % 10;
			hashedPassword += hashedChar;
		}
	}
	return hashedPassword;
}
