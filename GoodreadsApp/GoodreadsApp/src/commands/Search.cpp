#include "../../include/commands/Search.h"
#include "../../include/commands/CommandUtils.h"

using namespace Helper;

Search::Search(BookService& b, SocialService& s) : bookService(b), socialService(s) {}

void Search::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2) 
	{ 
		fail("Error: Usage: search <name>");
	}
	if (isBlank(args[1]))
	{
		fail("Error: Search name cannot be empty.");
	}
	socialService.searchUser(args[1]);
	std::cout << "\n";
	bookService.searchBooks(args[1]);
}
