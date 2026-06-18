#include "../../include/commands/ShowInbox.h"
#include "../../include/commands/CommandUtils.h"

ShowInbox::ShowInbox(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void ShowInbox::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		fail("Error: Usage: show-inbox [filter]");
	}
	socialService.showInbox(args.size() == 2 ? args[1] : "");
}
