#pragma once
#include <string>

namespace ValidationUtils
{
	void validateUsername(const std::string& username);
	void validatePassword(const std::string& password);
	void validateCredentials(const std::string& username, const std::string& password);
}
