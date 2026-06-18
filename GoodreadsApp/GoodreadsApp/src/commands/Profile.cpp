#include "../../include/commands/Profile.h"
#include "../../include/commands/CommandUtils.h"

Profile::Profile(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Profile::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		fail("Error: Usage: profile [reader]");
	}
	socialService.showProfile(args.size() == 2 ? args[1] : "");
}
