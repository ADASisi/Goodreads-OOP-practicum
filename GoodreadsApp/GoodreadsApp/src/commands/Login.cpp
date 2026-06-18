#include "../../include/commands/Login.h"
#include "../../include/commands/CommandUtils.h"

Login::Login(AuthService& authService) : authService(authService)
{
}

void Login::execute(const std::vector<std::string>& args)
{
	if (args.size() != 3) {
		fail("Error: Usage: login <username> <password>");
	}

	if (authService.login(args[1], args[2])) {
		std::cout << "Login successful.\n";
	}
	else {
		fail("Error: User not found, invalid password, or another user is already logged in.");
	}
}
