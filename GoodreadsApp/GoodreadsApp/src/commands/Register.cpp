#include "../../include/commands/Register.h"
#include "../utils/HelperFunctions.h"
#include <iostream>

Register::Register(AuthService& authService) : authService(authService) {}

void Register::execute(const std::vector<std::string>& args)
{
	if (args.size() != 4)
	{
		std::cout << "Error: Usage: register <username> <password> <reader|author|publisher>\n";
		return;
	}
	TypeUsers type;
	std::string value = toLower(args[3]);
	if (value == "reader") type = TypeUsers::Reader;
	else if (value == "author") type = TypeUsers::Author;
	else if (value == "publisher") type = TypeUsers::Publisher;
	else 
	{ 
		std::cout << "Error: Invalid user type.\n"; 
		return; 
	}

	if (authService.registerUser(args[1], args[2], type)) std::cout << "User registered successfully.\n";
	else std::cout << "Error: Username already exists.\n";
}
