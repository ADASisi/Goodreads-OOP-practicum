#include "../../include/commands/AddBirthday.h"
#include "../../include/commands/CommandUtils.h"

AddBirthday::AddBirthday(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void AddBirthday::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		fail("Error: Usage: add-birthday [date]");
	}
	socialService.addBirthday(args.size() == 2 ? args[1] : "");
}
