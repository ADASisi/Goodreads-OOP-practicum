#include "../../include/commands/RemoveFromShelf.h"
#include "../../include/commands/CommandUtils.h"

using namespace Helper;

RemoveFromShelf::RemoveFromShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void RemoveFromShelf::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 3) 
	{ 
		fail("Error: Usage: remove-from-shelf <bookName> <shelfName>");
	}
	if (!isValidBookTitle(args[1]))
	{
		fail("Error: Book name must be 1 to 100 characters long.");
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	bookService.removeBookFromShelf(r, args[1], args[2]);
}
