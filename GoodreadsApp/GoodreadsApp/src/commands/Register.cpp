#include "../../include/commands/Register.h"
#include "../../include/commands/CommandUtils.h"
#include <iostream>

Register::Register(AuthService& authService) : authService(authService) {}

void Register::execute(const std::vector<std::string>& args)
{
	if (args.size() != 4)
	{
		fail("Error: Usage: register <username> <password> <reader|author|publisher>");
	}
	if (!isValidUsername(args[1]))
	{
		fail("Error: Username must be 6 to 24 characters long.");
	}
	if (!isValidPassword(args[2]))
	{
		fail(getPasswordValidationError(args[2]));
	}
	TypeUsers type;
	std::string value = toLower(args[3]);
	bool succesfullyParsed = parseUserType(value, type);
	if(!succesfullyParsed)
	{ 
		fail("Error: Invalid user type.");
	}

	if (authService.registerUser(args[1], args[2], type)) std::cout << "User registered successfully.\n";
	else fail("Error: Username already exists.");
}
