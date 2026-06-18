#include "../../include/commands/AddBirthday.h"
#include <iostream>

AddBirthday::AddBirthday(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void AddBirthday::execute(const std::vector<std::string>& args) 
{
	if (args.size() > 2) 
	{ 
		std::cout << "Error: Usage: add-birthday [date]\n"; 
		return; 
	}
	socialService.addBirthday(args.size() == 2 ? args[1] : "");
}
