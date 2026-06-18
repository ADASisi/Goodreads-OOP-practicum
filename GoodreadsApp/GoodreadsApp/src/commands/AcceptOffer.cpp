#include "../../include/commands/AcceptOffer.h"
#include "../../include/commands/CommandUtils.h"

AcceptOffer::AcceptOffer(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void AcceptOffer::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		fail("Error: Usage: accept-offer <index>");
	}
	int index; 
	parseMessageIndex(args[1], index);
	socialService.acceptOffer(index);
}
