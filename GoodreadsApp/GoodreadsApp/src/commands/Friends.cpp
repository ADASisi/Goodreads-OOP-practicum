#include "../../include/commands/Friends.h"
#include <iostream>

Friends::Friends(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Friends::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		std::cout << "Error: Usage: friends [reader]\n"; 
		return; 
	}
	socialService.showFriends(args.size() == 2 ? args[1] : "");
}
