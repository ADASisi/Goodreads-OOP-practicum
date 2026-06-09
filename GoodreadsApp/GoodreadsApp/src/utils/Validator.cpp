#include "../../include/utils/Validator.h"
#include<string>

static bool isValidUsername(const string& username)
{
	if (username.length() < 6 || username.length() > 24)
		return false;
	return true;
}

static bool isValidPasswod(const string& password)
{
	if (password.length() < 12 || password.length() > 36) return false;
	bool hasLower = false, hasUpper = false, hasSpecial = false;
	for (char c : password) {
		if (islower(c)) hasLower = true;
		else if (isupper(c)) hasUpper = true;
		else if (!isalpha(c)) hasSpecial = true;
	}
	return hasLower && hasUpper && hasSpecial;
}
