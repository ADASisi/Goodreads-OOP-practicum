#include "../../include/commands/ShowShelf.h"
#include "../../include/commands/CommandUtils.h"

ShowShelf::ShowShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void ShowShelf::execute(const std::vector<std::string>& args) 
{
	Reader* current = requireReader(authService, "Error: Reader or author login required.");
	if (args.size() == 2) 
	{ 
		bookService.showShelf(current, current, args[1]);
		return; 
	}
	if (args.size() != 3) 
	{ 
		fail("Error: Usage: show-shelf [reader] <name>");
	}
	Reader* target = dynamic_cast<Reader*>(findUser(authService, args[1]));
	if (!target) 
	{ 
		fail("Error: Reader not found.");
	}
	if (!current->isFollowing(target) || !target->isFollowing(current)) 
	{
		fail("Error: Access denied. You must be friends.");
	}
	bookService.showShelf(current, target, args[2]);
}
