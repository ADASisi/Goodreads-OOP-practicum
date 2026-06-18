#include "../../include/commands/LeavePublisher.h"
#include "../../include/commands/CommandUtils.h"

LeavePublisher::LeavePublisher(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void LeavePublisher::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		fail("Error: Usage: leave <publisher>");
	}
	socialService.leavePublisher(args[1]);
}
