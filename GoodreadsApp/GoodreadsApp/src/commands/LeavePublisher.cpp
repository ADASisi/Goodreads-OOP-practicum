#include "../../include/commands/LeavePublisher.h"
#include <iostream>

LeavePublisher::LeavePublisher(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void LeavePublisher::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: leave <publisher>\n"; 
		return; 
	}
	socialService.leavePublisher(args[1]);
}
