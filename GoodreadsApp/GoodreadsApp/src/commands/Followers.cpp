#include "../../include/commands/Followers.h"
#include "../../include/commands/CommandUtils.h"
#include <iostream>

Followers::Followers(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Followers::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 1) 
	{ 
		std::cout << "Error: Usage: followers\n"; 
		return; 
	}
	Author* author = requireAuthor(authService, "Error: Only authors can view followers with this command.");
	if (author) socialService.showFollowers(author);
}
