#include "../../include/commands/Login.h"
#include <iostream>

Login::Login(AuthService& authService) : authService(authService)
{
}

void Login::execute(const std::vector<std::string>& args)
{
	if (args.size() != 3) {
		std::cout << "Error: Usage: login <username> <password>\n";
		return;
	}

	if (authService.login(args[1], args[2])) {
		std::cout << "Login successful.\n";
	}
	else {
		std::cout << "Error: User not found, invalid password, or another user is already logged in.\n";
	}
}
