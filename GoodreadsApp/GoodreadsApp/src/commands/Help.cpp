#include "../../include/commands/Help.h"
#include "../../include/commands/CommandUtils.h"
#include <iostream>

Help::Help(AuthService& authService) : authService(authService) {}

void Help::execute(const std::vector<std::string>& args)
{
	if (args.size() != 1) {
		fail("Error: Usage: help");
	}

	std::cout << "Allowed commands:\n";
	std::cout << "- help\n";

	User* currentUser = authService.getCurrentUser();
	if (!currentUser) {
		std::cout << "- register <username> <password> <reader|author|publisher>\n";
		std::cout << "- login <username> <password>\n";
		std::cout << "- exit\n";
		return;
	}

	std::cout << "- logout\n";
	std::cout << "- exit\n";

	if (currentUser->getType() == TypeUsers::Reader) {
		std::cout << "- search <name>\n";
		std::cout << "- follow <username>\n";
		std::cout << "- add-book <bookName> <status> [rating]\n";
		std::cout << "- create-shelf <name>\n";
		std::cout << "- delete-shelf <name>\n";
		std::cout << "- add-to-shelf <bookName> <shelfName>\n";
		std::cout << "- remove-from-shelf <bookName> <shelfName>\n";
		std::cout << "- delete-book <bookName>\n";
		std::cout << "- show-shelf [reader] <name>\n";
		std::cout << "- show-inbox [filter]\n";
		std::cout << "- read-msg <index>\n";
		std::cout << "- delete-msg <index>\n";
		std::cout << "- friends [reader]\n";
		std::cout << "- add-birthday [date]\n";
		std::cout << "- profile [reader]\n";
	}
	else if (currentUser->getType() == TypeUsers::Publisher) {
		std::cout << "- publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>\n";
		std::cout << "- add-synopsis <bookTitle> <synopsis>\n";
		std::cout << "- offer <author>\n";
	}
	else if (currentUser->getType() == TypeUsers::Author) {
		std::cout << "- show-inbox [filter]\n";
		std::cout << "- accept-offer <index>\n";
		std::cout << "- leave <publisher>\n";
		std::cout << "- followers\n";
	}
}
