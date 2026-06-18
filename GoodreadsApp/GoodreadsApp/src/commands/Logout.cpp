#include "../../include/commands/Logout.h"
#include "../../include/commands/CommandUtils.h"

LogOut::LogOut(AuthService& authService) : authService(authService)
{
}

void LogOut::execute(const std::vector<std::string>& args)
{
	if (args.size() != 1) {
		fail("Error: Usage: logout");
	}
	if (!authService.getCurrentUser()) {
		fail("Error: No user is currently logged in.");
	}

	authService.logout();
	std::cout << "Logout successful.\n";
}
