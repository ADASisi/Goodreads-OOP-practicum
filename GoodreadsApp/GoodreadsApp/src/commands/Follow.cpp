#include "../../include/commands/Follow.h"
#include "../../include/commands/CommandUtils.h"
#include <iostream>

Follow::Follow(AuthService& authService, SocialService& socialService)
	: authService(authService), socialService(socialService) {}

void Follow::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: follow <username>\n"; 
		return; 
	}
	User* targetUser = findUser(authService, args[1]);
	if (!targetUser) {
		std::cout << "Error: User '" << args[1] << "' not found.\n";
		return;
	}
	socialService.followUser(targetUser);
}
