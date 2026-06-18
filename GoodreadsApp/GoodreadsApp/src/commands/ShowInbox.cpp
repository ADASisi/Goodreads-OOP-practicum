#include "../../include/commands/ShowInbox.h"
#include <iostream>

ShowInbox::ShowInbox(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void ShowInbox::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		std::cout << "Error: Usage: show-inbox [filter]\n"; 
		return; 
	}
	socialService.showInbox(args.size() == 2 ? args[1] : "");
}
