#include "../../include/commands/Friends.h"
#include "../../include/commands/CommandUtils.h"

Friends::Friends(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Friends::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		fail("Error: Usage: friends [reader]");
	}
	socialService.showFriends(args.size() == 2 ? args[1] : "");
}
