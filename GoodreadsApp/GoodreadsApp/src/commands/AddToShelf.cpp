#include "../../include/commands/AddToShelf.h"
#include "../../include/commands/CommandUtils.h"

AddToShelf::AddToShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void AddToShelf::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 3) 
	{ 
		fail("Error: Usage: add-to-shelf <bookName> <shelfName>");
	}
	if (!isValidBookTitle(args[1]))
	{
		fail("Error: Book name must be 1 to 100 characters long.");
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	bookService.addBookToShelf(r, args[1], args[2]);
}
