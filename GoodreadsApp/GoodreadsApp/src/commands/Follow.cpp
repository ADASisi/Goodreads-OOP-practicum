#include "../../include/commands/Follow.h"
#include "../../include/commands/CommandUtils.h"
#include <iostream>

Follow::Follow(AuthService& authService, SocialService& socialService)
	: authService(authService), socialService(socialService) {}

void Follow::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2) 
	{ 
		fail("Error: Usage: follow <username>");
	}
	User* targetUser = findUser(authService, args[1]);
	if (!targetUser) {
		fail("Error: User '" + args[1] + "' not found.");
	}
	socialService.followUser(targetUser);
}
