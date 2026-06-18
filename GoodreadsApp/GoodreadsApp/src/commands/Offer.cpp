#include "../../include/commands/Offer.h"
#include <iostream>

Offer::Offer(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void Offer::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2)
	{ 
		std::cout << "Error: Usage: offer <author>\n"; 
		return; 
	}
	socialService.sendOffer(args[1]);
}
