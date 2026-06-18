#include "../../include/commands/Profile.h"
#include <iostream>

Profile::Profile(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Profile::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		std::cout << "Error: Usage: profile [reader]\n"; 
		return; 
	}
	socialService.showProfile(args.size() == 2 ? args[1] : "");
}
