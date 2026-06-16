#include "../../include/utils/ValidationUtils.h"
#include "../../include/utils/ValidationExceptions.h"
#include <cctype>

ValidationException::ValidationException(const std::string& message)
	: std::runtime_error(message)
{
}

InvalidUsernameException::InvalidUsernameException(const std::string& message)
	: ValidationException(message)
{
}

InvalidPasswordException::InvalidPasswordException(const std::string& message)
	: ValidationException(message)
{
}

namespace
{
	bool hasLowercaseLetter(const std::string& value)
	{
		for (unsigned char ch : value) {
			if (std::islower(ch)) {
				return true;
			}
		}
		return false;
	}

	bool hasUppercaseLetter(const std::string& value)
	{
		for (unsigned char ch : value) {
			if (std::isupper(ch)) {
				return true;
			}
		}
		return false;
	}

	bool hasNonLetterSymbol(const std::string& value)
	{
		for (unsigned char ch : value) {
			if (!std::isalpha(ch)) {
				return true;
			}
		}
		return false;
	}
}

void ValidationUtils::validateUsername(const std::string& username)
{
	if (username.length() < 6 || username.length() > 24) {
		throw InvalidUsernameException("Username must be between 6 and 24 characters long.");
	}
}

void ValidationUtils::validatePassword(const std::string& password)
{
	if (password.length() < 12 || password.length() > 36) {
		throw InvalidPasswordException("Password must be between 12 and 36 characters long.");
	}

	if (!hasLowercaseLetter(password)) {
		throw InvalidPasswordException("Password must contain at least one lowercase letter.");
	}

	if (!hasUppercaseLetter(password)) {
		throw InvalidPasswordException("Password must contain at least one uppercase letter.");
	}

	if (!hasNonLetterSymbol(password)) {
		throw InvalidPasswordException("Password must contain at least one non-letter symbol.");
	}
}

void ValidationUtils::validateCredentials(const std::string& username, const std::string& password)
{
	validateUsername(username);
	validatePassword(password);
}
