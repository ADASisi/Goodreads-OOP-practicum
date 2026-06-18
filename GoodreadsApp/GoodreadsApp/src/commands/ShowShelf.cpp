#include "../../include/commands/ShowShelf.h"
#include "../../include/commands/CommandUtils.h"

ShowShelf::ShowShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void ShowShelf::execute(const std::vector<std::string>& args) 
{
	Reader* current = requireReader(authService, "Error: Reader or author login required.");
	if (!current) return;
	if (args.size() == 2) 
	{ 
		bookService.showShelf(current, current, args[1]);
		return; 
	}
	if (args.size() != 3) 
	{ 
		std::cout << "Error: Usage: show-shelf [reader] <name>\n"; 
		return; 
	}
	Reader* target = dynamic_cast<Reader*>(findUser(authService, args[1]));
	if (!target) 
	{ 
		std::cout << "Error: Reader not found.\n"; 
		return; 
	}
	if (!current->isFollowing(target) || !target->isFollowing(current)) 
	{
		std::cout << "Error: Access denied. You must be friends.\n"; 
		return;
	}
	bookService.showShelf(current, target, args[2]);
}
