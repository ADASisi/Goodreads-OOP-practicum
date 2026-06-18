#include "../../include/commands/Logout.h"
#include <iostream>

LogOut::LogOut(AuthService& authService) : authService(authService)
{
}

void LogOut::execute(const std::vector<std::string>& args)
{
	if (args.size() != 1) {
		std::cout << "Error: Usage: logout\n";
		return;
	}
	if (!authService.getCurrentUser()) {
		std::cout << "Error: No user is currently logged in.\n";
		return;
	}

	authService.logout();
	std::cout << "Logout successful.\n";
}
