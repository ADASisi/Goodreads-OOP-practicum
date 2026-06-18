#include "../../include/commands/Offer.h"
#include "../../include/commands/CommandUtils.h"

Offer::Offer(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Offer::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2)
	{ 
		fail("Error: Usage: offer <author>");
	}
	socialService.sendOffer(args[1]);
}
